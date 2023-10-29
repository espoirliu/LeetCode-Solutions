// Time:  O(nlogn)
// Space: O(n)

// dp, segment tree, math
class Solution {
public:
    int sumCounts(vector<int>& nums) {
        static const int MOD = 1e9 + 7;

        const auto& sum = [&] (const auto& x, const auto& y) {
            return (x + y) % MOD;
        };

        int result = 0, accu = 0;
        SegmentTree<int> st(size(nums), sum, sum);
        unordered_map<int, int> lookup;
        for (int i = 0; i < size(nums); ++i) {
            const int j = lookup.count(nums[i]) ? lookup[nums[i]] : -1;
            // sum(count(k, i)^2 for k in range(i+1)) - sum(count(k, i-1)^2 for k in range(i))
            // = sum(2*count(k, i-1)+1 for k in range(j+1, i+1))
            // = (i-j) + sum(2*count(k, i-1) for k in range(j+1, i+1))
            accu = (accu + ((i - j) + 2ll * st.query(j + 1, i))) % MOD;
            result = (result + accu) % MOD;
            st.update(j + 1, i, 1);  // count(k, i) = count(k, i-1)+(1 if k >= j+1 else 0) for k in range(i+1)
            lookup[nums[i]] = i;
        }
        return result;
    }

private:
    template <typename T>
    class SegmentTree {
    public:
        explicit SegmentTree(
            int N,
            const function<T(const T&, const T&)>& query_fn,
            const function<T(const T&, const T&)>& update_fn)
         :  base_(N > 1 ? 1 << (__lg(N - 1) + 1) : 1),
            lazy_(base_),
            tree_(N > 1 ? 1 << (__lg(N - 1) + 2) : 2),
            count_(size(tree_), 1),
            query_fn_(query_fn),
            update_fn_(update_fn) {

            for (int i = base_ - 1; i >= 1; --i) { // added
                count_[i] = count_[i << 1] + count_[(i << 1) + 1];
            }
        }

        void update(int L, int R, const T& val) {
            L += base_;
            R += base_;
            // push(L);  // enable if range assignment
            // push(R);  // enable if range assignment
            int L0 = L, R0 = R;
            for (; L <= R; L >>= 1, R >>= 1) {
                if ((L & 1) == 1) {
                    apply(L++, val);
                }
                if ((R & 1) == 0) {
                    apply(R--, val);
                }
            }
            pull(L0);
            pull(R0);
        }

        T query(int L, int R) {
            if (L > R) {
                return T{};
            }
            L += base_;
            R += base_;
            push(L);
            push(R);
            T left{}, right{};
            for (; L <= R; L >>= 1, R >>= 1) {
                if ((L & 1) == 1) {
                    left = query_fn_(tree_[L++], left);
                }
                if ((R & 1) == 0) {
                    right = query_fn_(right, tree_[R--]);
                }
            }
            return query_fn_(left, right);
        }

    private:
        void apply(int x, const T val) {
            tree_[x] = update_fn_(tree_[x], val * count_[x]);  // modified
            if (x < base_) {
                lazy_[x] = update_fn_(lazy_[x], val);
            }
        }

        void pull(int x) {
            while (x > 1) {
                x >>= 1;
                tree_[x] = query_fn_(tree_[x << 1], tree_[(x << 1) + 1]);
                if (lazy_[x]) {
                    tree_[x] = update_fn_(tree_[x], lazy_[x] * count_[x]);  // modified
                }
            }
        }

        void push(int x) {
            for (int h = __lg(base_) - 1; h > 0; --h) {
                int y = x >> h;
                if (lazy_[y]) {
                    apply(y << 1, lazy_[y]);
                    apply((y << 1) + 1, lazy_[y]);
                    lazy_[y] = 0;
                }
            }
        }

        int base_;
        vector<T> tree_;
        vector<T> lazy_;
        vector<T> count_;  // added
        const function<T(const T&, const T&)> query_fn_;
        const function<T(const T&, const T&)> update_fn_;
    };
};
