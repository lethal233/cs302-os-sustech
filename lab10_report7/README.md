# Report7



- Briefly describe the FIFO page-replacement algorithm and analyze its algorithm complexity

  - hit: return the page result in the cache. miss: if the queue is full, pop the head (the earliest page put in the cache) of the queue, and push the page into the cache. Else put the required page into the cache. time complexity: O(1) for updating the cache, O(n) for searching for the page space complexity: O(k) for the cache with k pages

  

- Briefly describe the MIN page-replacement algorithm and analyze its algorithm complexity

  - Hit: return the page result miss: if the cache is full, the page that will be used in the longest time will be removed and the corresponding page will be put into the cache. Else just put the page into the cache. Time complexity: O(log n) for updating the page (using priority queue), total is `O(nlogn)` space complexity: O(n) for storing the pages

  

- Briefly describe the LRU page-replacement algorithm and analyze its algorithm complexity

  - hit: move the hit page to the head/top of the cache and return the hit page. miss: if the cache is full, remove the least recent use page in the cache and put the corresponding page into the head/top of the cache. Else put the corresponding page into the head/top of the cache. Time complexity: `O(n)` for each update Space complexity: `O(n)`

  

- Briefly describe the clock algorithm and analyze its algorithm complexity

  - hit: return the page in the cache and set the valid bit for the page as 1.miss: if the valid bit of the page that the pointer points to is 1, then set the valid bit as 0 and move the pointer to the next page and do the same operation until the valid bit of the page that the pointer points to is already 0. If the page that the pointer points to already has valid bit 0, then replace the corresponding page here and set the valid bit as 1. Time complexity: O(n) for searching the update Space complexity: O(n) for storing

  

- Answer the hit rate

  |      | FIFO   | MIN    | LRU    | CLOCK  |
  | ---- | ------ | ------ | ------ | ------ |
  | 1.in | 11.98% | 42.40% | 11.76% | 11.93% |
  | 2.in | 11.85% | 43.27% | 11.85% | 11.83% |
  | 3.in | 82.36% | 88.58% | 82.39% | 82.38% |

    

