#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <functional>
#include <vector>
#include <utility>
#include <numeric>

namespace functionalcpp {

//todo: split out, write tests (what is a good c++ test library?)
// inspired by: https://play.kotlinlang.org/koans/Collections/Sum/Task.kt

    template<typename a, typename b, typename c>
    inline std::unordered_map<b, c> associate(std::vector<a> &items, std::function<std::pair<b, c>(a&)> f) {
        std::unordered_map<b, c> map = {};
        for (auto &item : items) {
            auto pair = f(item);
            auto key = pair.first;
            auto value = pair.second;
            map[key] = value;
        }
        return map;
    }

    //todo: associateBy
    template<typename a, typename b>
    inline std::unordered_map<b,a> associateBy(std::vector<a> &items, std::function<b (a&)> f) {
        return associate(items, [f](a &item){ return pair<b,a>(f(item), item); } );
    }

    //todo: test
    template<typename a, typename b>
    inline std::unordered_map<a,b> associateWith(std::vector<a> &items, std::function<b (a&)> f) {
        return associate(items, [f](a &item){ return std::pair<a,b>(item, f(item)); } );
    }

    //todo: distinctBy?
    // https://kotlinlang.org/api/latest/jvm/stdlib/kotlin.sequences/
    // todo: drop
    // todo: take

    //todo: zip
//    template<typename a, typename b>
//    inline std::unordered_map<a,b>

    //note:: std::all_of, std::any_of, std::none_of (could do inline & pass through with iterators)
    template<typename a>
    inline bool all(std::vector<a> &items, std::function<bool(a&)> f) {
        return std::all_of(items.begin(), items.end(), f);
    }

/*
    template<typename a>
    bool operator|(const std::vector<a>& lhs, const std::function<std::vector<a>> rhs)
    {
        return lhs.num() == rhs.num() && lhs.den() == rhs.den();
    }*/

    template<typename a, typename b>
    inline std::unordered_map<b, std::vector<a>> groupBy(std::vector<a> &items, std::function<b(a&)> f) {
        std::unordered_map<b, std::vector<a>> map = {};
        for (auto &item : items) {
            auto key = f(item);
            if (map.find(key) == map.end()) {
                map[key] = std::vector<a>();
            }
            map[key].push_back(item);
        }
        return map;
    }

    template<typename a>
    inline int sumBy(std::vector<a> &items, std::function<int(a&)> f) {
        int sum = 0;
        for (auto item : items) {
            int add = f(item);
            sum += add;
        }
        return sum;
    }

    template<typename a>
    inline void with(a &it, std::function<void(a &)> f) {
        f(it);
    }

    template<typename a, typename b>
    inline std::vector<b> map(std::vector<a> &items, std::function<std::vector<b>(a)> f) {
        std::vector<b> list;
        list.resize(items.size());
        transform(items.begin(), items.end(), list.begin(), f);
        return list;
    }

    //todo: test
    template<typename a, typename b>
    inline std::vector<b> flatMap(std::vector<a> &items, std::function<std::vector<b>(a)> f) {
        std::vector<b> list;
        auto sublist = map(items, f);
        list.insert(end(list), begin(sublist), end(sublist));
        return list;
    }

    template<typename a>
    inline std::vector<a> filter(std::vector<a> &items, std::function<bool(a)> f) {
        std::vector<a> list;
        list.resize(items.size());
        std::copy_if(items.begin(), items.end(), list.begin(), f);
        return list;
    }

    // todo: reduce
    template<typename a>
    inline a reduce( std::vector<a> &items, std::function<a(a, a&)> f){
        if( items.size() == 0 ) return a();
        a result = items.begin();
        for( auto i : items.begin()++ ) { // todo: huh?
            //todo: start with first, fold down f()...
        }
        //TODO("")
//        return std::reduce( items.start(), items.end(), f );
    }
    template<typename a, typename b>
    inline b fold( b initial, std::vector<a> items, std::function<a(b, a&)> f ) {
        b result = initial;
        for( auto &item : items ) {
            result = f( result, item );
        }
        return result;
//        return std::accumulate()
    }

}


struct Person {
    int id;
    std::string name;
    double age;
};
int main() {
    std::vector<Person> persons = {{1, "Yes",       3.0},
                                   {2, "No",        42.0},
                                   {3, "Yogi Bear", 33.0}};
    auto myMap = functionalcpp::associate<Person, std::string, int>(persons, [](auto person) {
        return std::pair<std::string, int>(person.name, person.id);
    });
    std::string test = "tesT";
    for (auto &it : myMap) {
//        with(it, [](auto it) { });
        std::cout << it.first.c_str() << " " << it.second;
    }
    std::cout << "Hello, World!" << std::endl;

    //Change the age of persons[0], using with, a bit excessive here, but if we had a lot of things to change, it would be more reasonable
    functionalcpp::with<Person>(persons[0], [](auto &it) {
        it.age = 42.0;
    });

    // Using a groupBy
    auto grouped = functionalcpp::groupBy<Person, double>(persons, [](auto person) { return person.age; });

    for (auto &it : grouped) {
        std::cout << "Grouped under age -> " << it.first << ":";
        for (auto &person: it.second) {
            std::cout << " " << person.name.c_str();
        }
        std::cout << std::endl;
    }

    return 0;
}
