#include "PeerPilotSurveyReader.h"

// PeerReview
PeerReview::PeerReview(int id, const std::string& name) : originId(id), peerName(name) {}

void PeerReview::setOriginId(int id) {
    originId = id;
}

void PeerReview::setPeerName(const std::string& name) {
    peerName = name;
}

void PeerReview::addAnswer(const std::string& answer) {
    answers.push_back(answer);
}

int PeerReview::getOriginId() const {
    return originId;
}

std::string PeerReview::getPeerName() const {
    return peerName;
}

std::vector<std::string> PeerReview::getAnswers() const {
    return answers;
}

void PeerReview::print() const {
    std::cout << "Peer Name: " << peerName << std::endl;
    std::cout << "Origin ID: " << originId << std::endl;
    std::cout << "Answers:" << std::endl;
    for (const auto& answer : answers) {
        std::cout << "- " << answer << std::endl;
    }
}

// Response
Response::Response(const std::string& n, int i) : name(n), id(i) {}

void Response::setName(const std::string& n) {
    name = n;
}

void Response::setId(int i) {
    id = i;
}

void Response::addPeerReview(const PeerReview& response) {
    peerReviews.push_back(response);
}

void Response::removeFirstPeerReview() {
    if (!peerReviews.empty()) {
        peerReviews.erase(peerReviews.begin());
    }
}

std::string Response::getName() const {
    return name;
}

int Response::getId() const {
    return id;
}

std::vector<PeerReview> Response::getPeerReviews() const {
    return peerReviews;
}

void Response::print() const {
    std::cout << "Response Name: " << name << std::endl;
    std::cout << "Response ID: " << id << std::endl;
    std::cout << "Peer Reviews:" << std::endl;
    for (const auto& peerReview : peerReviews) {
        std::cout << "- Peer Name: " << peerReview.getPeerName() << std::endl;
        std::cout << "  Origin ID: " << peerReview.getOriginId() << std::endl;
        std::cout << "  Answers:" << std::endl;
        for (const auto& answer : peerReview.getAnswers()) {
            std::cout << "  - " << answer << std::endl;
        }
    }
}

void Response::replaceName(const std::string& oldName, const std::string& newName) {
    for (auto& review : peerReviews) {
        if (review.getPeerName() == oldName) {
            review.setPeerName(newName);
        }
    }
}

// ResponseList
void ResponseList::addResponse(const Response& response) {
    responses.push_back(response);
}

std::vector<PeerReview> ResponseList::getPeerReviewsByPeerName(const std::string& peerNameInput) const {
    std::vector<PeerReview> matchingPeerReviews;
    std::string peerName = reformatName(peerNameInput);
    for (const auto& response : responses) {
        for (const auto& peerReview : response.getPeerReviews()) {
            if (peerReview.getPeerName() == peerName) {
                matchingPeerReviews.push_back(peerReview);
            }
        }
    }
    return matchingPeerReviews;
}

std::vector<Response> ResponseList::getResponses() const {
    return responses;
}

std::vector<std::string> ResponseList::getUnmatchedNames(const std::vector<std::string>& names) const {
    std::vector<std::string> unmatchedNames;

    // Collect all peer names from peer reviews
    std::vector<std::string> allPeerNames;
    for (const auto& response : responses) {
        for (const auto& peerReview : response.getPeerReviews()) {
            allPeerNames.push_back(peerReview.getPeerName());
        }
    }

    // Find unmatched names
    for (const auto& peerName : allPeerNames) {
        if (std::find(names.begin(), names.end(), peerName) == names.end()) {
            if (std::find(unmatchedNames.begin(), unmatchedNames.end(), peerName) == unmatchedNames.end())
            {
                //continue;
            }
            unmatchedNames.push_back(peerName);
            //std::cout << peerName << std::endl;
        }
    }

    return unmatchedNames;
}

void ResponseList::replaceName(const std::string& oldName, const std::string& newNameInput) {
    std::string newName = reformatName(newNameInput);
    for (auto& resp : responses) {
        resp.replaceName(oldName, newName);
    }
}

bool is_number(const std::string& s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

std::vector<std::vector<std::string>> parseCSV(const std::string& filename) {
csv:csv::CSVReader reader(filename);

    std::ifstream file(filename);
    std::vector<std::vector<std::string>> data;

    for (csv::CSVRow& rowCSV: reader) { // Input iterator
        if(data.empty()){
            data.push_back(rowCSV.get_col_names());
        }
        std::vector<std::string> row;
        for (csv::CSVField& fieldCSV: rowCSV) {
            row.push_back(fieldCSV.get<>());
        }
        data.push_back(row);
    }

    return data; // File closes automatically when 'file' goes out of scope
}

std::vector<std::vector<std::string>> removePreviousAttempts(const std::vector<std::vector<std::string>>& data, int idcol, int attemptcol) {
    std::unordered_map<std::string, int> idToMaxAttemptIndex;

    for (int i = 1; i < data.size(); i++) {
        const std::string& id = data[i][idcol];
        int attempt = std::stoi(data[i][attemptcol]);

        if (idToMaxAttemptIndex.find(id) == idToMaxAttemptIndex.end() || attempt > std::stoi(data[idToMaxAttemptIndex[id]][attemptcol])) {
            idToMaxAttemptIndex[id] = i;
        }
    }

    std::vector<std::vector<std::string>> uniqueRows;
    for (const auto& pair : idToMaxAttemptIndex) {
        uniqueRows.push_back(data[pair.second]);
    }

    return uniqueRows;
}

std::string toLowercase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

ResponseList getData(std::string filePath) {
    std::vector<std::vector<std::string>> data = parseCSV(filePath);

    ResponseList responses;

    PeerReview titles = PeerReview(-1, "Titles");

    int nameCol = 0;
    int idCol = 1;

    std::vector<std::vector<std::string>> uniqueData = removePreviousAttempts(data, 1, 8);

    for (int i = 0; i < uniqueData.size(); i++) {
        Response* response = new Response(uniqueData[i][0], stoi(uniqueData[i][1]));
        PeerReview* peerReview = nullptr;
        bool firstPeerFound = false;

        for (int j = 0; j < uniqueData[i].size(); j++) {
            if (toLowercase(data[0][j]).find("peer name") != std::string::npos) {
                if (firstPeerFound) {
                    response->addPeerReview(*peerReview);
                }
                peerReview = new PeerReview(response->getId(), uniqueData[i][j]);
                firstPeerFound = true;
            }
            else if (is_number(data[0][j])) {
                continue;
            }
            else if (data[0][j].find("n correct") != std::string::npos) {
                if(firstPeerFound){
                    response->addPeerReview(*peerReview);
                }

            }
            else if (firstPeerFound) {
                peerReview->addAnswer(uniqueData[i][j]);
            }
        }

        responses.addResponse(*response);
    }
    return responses;
}


std::vector<std::string> getQuestionTitles(std::string filePath) {
    std::vector<std::vector<std::string>> data = parseCSV(filePath);

    std::vector<std::string> titles;

    int nameCol = 0;
    int idCol = 1;

    bool firstPeerFound = false;
    for (int j = 0; j < data[0].size(); j++) {
        if (toLowercase(data[0][j]).find("peer name") != std::string::npos) {
            if (firstPeerFound) {
                break;
            }
            firstPeerFound = true;
        }
        else if (is_number(data[0][j])) {
            continue;
        }
        else if (data[0][j].find("n correct") != std::string::npos) {
            break;
        }
        else if (firstPeerFound) {
            titles.push_back(data[0][j]);
        }
    }
    return titles;
}

std::string reformatName(const std::string& fullName) {
    std::string firstName, lastName;
    size_t commaPos = fullName.find(',');

    if (commaPos != std::string::npos) {
        lastName = fullName.substr(0, commaPos);
        firstName = fullName.substr(commaPos + 2); // Skip comma and space
        return firstName + " " + lastName;
    } else {
        // If there is no comma, return the original string
        return fullName;
    }
}

std::string getBestMatchingString(std::vector<std::string> names, std::string toMatch) {
    std::string closestMatch;
    int maxDistance = 2147483647;

    for (const auto& name : names) {
        int distance = levenshteinSSE::levenshtein(name, toMatch);
        if (distance < maxDistance) {
            maxDistance = distance;
            closestMatch = name;
            std::cout << name << " " << distance << std::endl;
        }
    }

    return closestMatch;
}

std::string makeSafeForCSV(const std::string& input) {
    std::string output = input;

    // Check if the string contains special characters
    if (output.find_first_of(",\"\n") != std::string::npos) {
        // Escape double quotes by doubling them
        size_t pos = output.find("\"");
        while (pos != std::string::npos) {
            output.replace(pos, 1, "\"\"");
            pos = output.find("\"", pos + 2);
        }

        // Enclose the string in double quotes
        output = "\"" + output + "\"";
    }

    return output;
}
