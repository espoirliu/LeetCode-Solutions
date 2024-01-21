# Time:  O(nlogn)
# Space: O(n)

from sortedcontainers import SortedList


# sliding window, sorted list
class Solution(object):
    def minimumCost(self, nums, k, dist):
        """
        :type nums: List[int]
        :type k: int
        :type dist: int
        :rtype: int
        """
        sl = SortedList(nums[1:1+dist])
        curr, mn = sum(sl[:k-2]), float("inf")
        for i in xrange(1+dist, len(nums)):
            curr += min(nums[i], sl[k-2] if k-2 < len(sl) else float("inf"))
            mn = min(mn, curr)
            sl.add(nums[i])
            curr -= min(nums[i-dist], sl[k-2] if k-2 < len(sl) else float("inf"))
            sl.remove(nums[i-dist])
        return nums[0]+mn