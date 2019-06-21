#include <vector>
#include <algorithm>

using std::vector;

template<typename Iterator>
Iterator get(Iterator start,Iterator end,int i){
    vector<Iterator> sorted; //contains the iterators of the given container.
    class Max{ //Functor for sorting the vector 'sorted'.
    public:
        bool operator()(Iterator a, Iterator b){
            return *a>*b; //return true if a points on a greater value than b.
        }
    };
    for(;start!=end; ++start){
        sorted.push_back(start);
    }
    sort(sorted.begin(),sorted.end(),Max());//sorts 'sorted' by Max()
    return (i-1>=0 && i-1<sorted.size())? sorted[i-1]: end;
}
