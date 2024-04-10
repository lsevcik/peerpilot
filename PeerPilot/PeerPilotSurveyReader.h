#ifndef PEERPILOT_PEERPILOTSURVEYREADER_H
#define PEERPILOT_PEERPILOTSURVEYREADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include <unordered_map>

class PeerReview {
private:
    int originId;
    std::string peerName;
    std::vector<std::string> answers;

public:
    PeerReview(int id, const std::string& name);
    void setOriginId(int id);
    void setPeerName(const std::string& name);
    void addAnswer(const std::string& answer);
    int getOriginId() const;
    std::string getPeerName() const;
    std::vector<std::string> getAnswers() const;
    void print() const;
};

class Response {
private:
    std::string name;
    int id;
    std::vector<PeerReview> peerReviews;

public:
    Response(const std::string& n, int i);
    void setName(const std::string& n);
    void setId(int i);
    void addPeerReview(const PeerReview& response);
    void removeFirstPeerReview();
    std::string getName() const;
    int getId() const;
    std::vector<PeerReview> getPeerReviews() const;
    void print() const;
};

class ResponseList {
private:
    std::vector<Response> responses;

public:
    void addResponse(const Response& response);
    std::vector<PeerReview> getPeerReviewsByPeerName(const std::string& peerName) const;
    std::vector<Response> getResponses() const;
};

bool is_number(const std::string& s);
std::vector<std::vector<std::string>> parseCSV(const std::string& filename);
std::vector<std::vector<std::string>> removePreviousAttempts(const std::vector<std::vector<std::string>>& data, int idcol, int attemptcol);
ResponseList getData(std::string filePath);
std::vector<std::string> getQuestionTitles(std::string filePath);

#endif //PEERPILOT_PEERPILOTSURVEYREADER_H
