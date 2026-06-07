#ifndef NAMESEARCH_SCORING_H
#define NAMESEARCH_SCORING_H

#include <string>
#include <vector>

std::string toLower(const std::string& s);
int scoreMatch(const std::string& nameLower, const std::string& queryLower);
std::vector<std::string> topK(const std::vector<std::string>& names, const std::string& query, int K);

#endif
