#include <vector>
#include <algorithm>

using std::vector;

template<typename Iterator,typename Functor>
Iterator get(const Iterator start, const Iterator end,int i,Functor compare){
    vector<Iterator> sorted; //contains the iterators of the given container.
    for(Iterator iterator = start;iterator!=end; ++iterator){
        sorted.push_back(iterator);
    }
    sort(sorted.begin(),sorted.end(),compare);
    return (i-1>=0 && i-1<int(sorted.size())) ? sorted[i-1] : end;
}
