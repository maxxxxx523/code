package javas;

import java.util.*;
import java.util.stream.Collectors;
class Solution {
    public int sumBase(int n, int k) {
        ArrayList<Integer> arr = new ArrayList<>();
        while (n > 0) {
            arr.add(n % k);
            n /= k;
        }
        int sum = arr.stream().collect(Collectors.summingInt(Integer::intValue));
        return sum;

    }

    public void main()
    {
        System.out.println(sumBase(54, 10));
    }
}