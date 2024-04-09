//
// Created by donov on 4/9/2024.
//
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

// ResponseList
void ResponseList::addResponse(const Response& response) {
    responses.push_back(response);
}

std::vector<PeerReview> ResponseList::getPeerReviewsByPeerName(const std::string& peerName) const {
    std::vector<PeerReview> matchingPeerReviews;
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

bool is_number(const std::string& s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

std::vector<std::vector<std::string>> parseCSV(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::vector<std::string>> data;

    if (!file.is_open()) {
        std::cerr << "Failed to open the file: " << filename << std::endl;
        return data;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string cell;
        while (std::getline(ss, cell, ',')) {
            row.push_back(cell);
        }
        data.push_back(row);
    }

    file.close();
    return data;
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
            if (data[0][j].find("Peer Name") != std::string::npos) {
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
                response->addPeerReview(*peerReview);
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
        if (data[0][j].find("Peer Name") != std::string::npos) {
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

/*
int main() {
    ResponseList responses = getData("data.csv");

    std::vector<std::string> titles = getQuestionTitles("data.csv");

    std::cout << "Questions:" << std::endl;
    for (auto& title : titles) {
        std::cout << title << std::endl;
    }
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "Responses:" << std::endl;
    for (auto& response : responses.getResponses()) {
        response.print();
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "All responses about John Smith:" << std::endl;
    std::vector<PeerReview> peerReviews = responses.getPeerReviewsByPeerName("John Smith");
    for (const auto& peerReview : peerReviews) {
        peerReview.print();
        std::cout << std::endl;
    }
    return 0;
}

*/