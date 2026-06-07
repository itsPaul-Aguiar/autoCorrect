#include "Scoring.h"

#include <algorithm>
#include <climits>
#include <queue>
#include <string>
#include <vector>

std::string toLower(const std::string& s)
{
    std::string out(s.size(), '\0');
    std::transform(s.begin(), s.end(), out.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return out;
}

int scoreMatch(const std::string& nameLower, const std::string& queryLower)
{
    if (queryLower.empty()) return 0;

    int qLen = static_cast<int>(queryLower.size());
    int nLen = static_cast<int>(nameLower.size());

    int qi = 0, firstPos = -1, lastPos = -1;
    int curRun = 0, bestRun = 0, prevNi = -2;

    for (int ni = 0; ni < nLen && qi < qLen; ++ni)
    {
        if (nameLower[ni] == queryLower[qi])
        {
            if (firstPos == -1) firstPos = ni;
            lastPos = ni;

            curRun = (ni == prevNi + 1) ? curRun + 1 : 1;
            if (curRun > bestRun) bestRun = curRun;
            prevNi = ni;
            ++qi;
        }
    }

    if (qi < qLen) return INT_MIN;

    int score = 0;

    if (nameLower.substr(0, static_cast<std::size_t>(qLen)) == queryLower)
        score += 100 * qLen;

    score += 10 * bestRun;
    score -= (lastPos - firstPos);
    score -= std::max(0, nLen - qLen);

    return score;
}

struct NameScore
{
    int score;
    std::string name;

    bool operator>(const NameScore& o) const { return score > o.score; }
};

std::vector<std::string> topK(const std::vector<std::string>& names,
                              const std::string& query, int K)
{
    if (query.empty()) return {};

    std::string qLow = toLower(query);

    std::priority_queue<NameScore,
                        std::vector<NameScore>,
                        std::greater<NameScore>> heap;

    for (const auto& name : names)
    {
        int s = scoreMatch(toLower(name), qLow);
        if (s == INT_MIN) continue;

        if (static_cast<int>(heap.size()) < K)
        {
            heap.push({s, name});
        }
        else if (s > heap.top().score)
        {
            heap.pop();
            heap.push({s, name});
        }
    }

    std::vector<NameScore> results;
    results.reserve(heap.size());
    while (!heap.empty())
    {
        results.push_back(heap.top());
        heap.pop();
    }

    std::sort(results.begin(), results.end(),
              [](const NameScore& a, const NameScore& b) { return a.score > b.score; });

    std::vector<std::string> out;
    out.reserve(results.size());
    for (auto& r : results) out.push_back(r.name);
    return out;
}
