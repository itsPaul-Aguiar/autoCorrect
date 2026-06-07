

# Scoring Explained

1. Perfect prefix bonus: +100×query length if name starts with query
2. Consecutive match run: +10× longest sequence of consecutive matching characters
3. Spread penalty: subtracts distance between first and last matched character
4. Length penalty: subtracts name length minus query length
Higher scores = better matches. The topK function uses a min-heap to keep the top K highest-scoring names.
