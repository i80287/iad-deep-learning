#include <algorithm>
#include <array>
#include <cassert>
#include <compare>
#include <cstdint>
#include <iostream>
#include <map>
#include <queue>
#include <vector>

int main() {
    using CountType = std::uint64_t;
    using YearType  = std::uint64_t;

    struct Entry {
        CountType count{};
        YearType year{};
    };

    const std::vector value_counts = {
        Entry{.count = 39404, .year = 2007}, Entry{.count = 37534, .year = 2006},
        Entry{.count = 34952, .year = 2005}, Entry{.count = 34760, .year = 2008},
        Entry{.count = 31038, .year = 2009}, Entry{.count = 29607, .year = 2004},
        Entry{.count = 27382, .year = 2003}, Entry{.count = 23451, .year = 2002},
        Entry{.count = 21590, .year = 2001}, Entry{.count = 19285, .year = 2000},
        Entry{.count = 18238, .year = 1999}, Entry{.count = 15814, .year = 1998},
        Entry{.count = 15182, .year = 1997}, Entry{.count = 14130, .year = 1996},
        Entry{.count = 13257, .year = 1995}, Entry{.count = 12121, .year = 1994},
        Entry{.count = 10525, .year = 1993}, Entry{.count = 9543, .year = 1992},
        Entry{.count = 9396, .year = 2010},  Entry{.count = 8647, .year = 1991},
        Entry{.count = 7256, .year = 1990},  Entry{.count = 6670, .year = 1989},
        Entry{.count = 5611, .year = 1988},  Entry{.count = 5122, .year = 1987},
        Entry{.count = 4219, .year = 1986},  Entry{.count = 3597, .year = 1982},
        Entry{.count = 3578, .year = 1985},  Entry{.count = 3386, .year = 1983},
        Entry{.count = 3368, .year = 1984},  Entry{.count = 3162, .year = 1981},
        Entry{.count = 3108, .year = 1979},  Entry{.count = 3101, .year = 1980},
        Entry{.count = 2926, .year = 1978},  Entry{.count = 2596, .year = 1973},
        Entry{.count = 2502, .year = 1977},  Entry{.count = 2482, .year = 1975},
        Entry{.count = 2349, .year = 1970},  Entry{.count = 2288, .year = 1972},
        Entry{.count = 2210, .year = 1969},  Entry{.count = 2184, .year = 1974},
        Entry{.count = 2179, .year = 1976},  Entry{.count = 2131, .year = 1971},
        Entry{.count = 1867, .year = 1968},  Entry{.count = 1718, .year = 1967},
        Entry{.count = 1377, .year = 1966},  Entry{.count = 1120, .year = 1965},
        Entry{.count = 945, .year = 1964},   Entry{.count = 902, .year = 1963},
        Entry{.count = 605, .year = 1962},   Entry{.count = 597, .year = 1957},
        Entry{.count = 592, .year = 1959},   Entry{.count = 583, .year = 1958},
        Entry{.count = 571, .year = 1961},   Entry{.count = 565, .year = 1956},
        Entry{.count = 424, .year = 1960},   Entry{.count = 275, .year = 1955},
        Entry{.count = 133, .year = 1953},   Entry{.count = 123, .year = 1954},
        Entry{.count = 93, .year = 1929},    Entry{.count = 83, .year = 1950},
        Entry{.count = 77, .year = 1952},    Entry{.count = 74, .year = 1951},
        Entry{.count = 60, .year = 1949},    Entry{.count = 57, .year = 1947},
        Entry{.count = 52, .year = 1940},    Entry{.count = 52, .year = 1928},
        Entry{.count = 43, .year = 1948},    Entry{.count = 42, .year = 1927},
        Entry{.count = 40, .year = 1930},    Entry{.count = 35, .year = 1939},
        Entry{.count = 35, .year = 1931},    Entry{.count = 32, .year = 1941},
        Entry{.count = 30, .year = 1945},    Entry{.count = 29, .year = 1946},
        Entry{.count = 29, .year = 1934},    Entry{.count = 28, .year = 1937},
        Entry{.count = 25, .year = 1936},    Entry{.count = 24, .year = 1942},
        Entry{.count = 24, .year = 1935},    Entry{.count = 19, .year = 1938},
        Entry{.count = 19, .year = 1926},    Entry{.count = 15, .year = 1944},
        Entry{.count = 14, .year = 1943},    Entry{.count = 11, .year = 1932},
        Entry{.count = 7, .year = 1925},     Entry{.count = 6, .year = 1922},
        Entry{.count = 6, .year = 1933},     Entry{.count = 5, .year = 1924},
        Entry{.count = 1, .year = 2011},
    };
    std::map<YearType, CountType> year_to_count;

    using ClassIndex = std::size_t;
    struct QueueEntry {
        CountType count{};
        ClassIndex class_index{};

        [[nodiscard]] constexpr std::strong_ordering operator<=>(
            const QueueEntry& other) const noexcept {
            return other.count <=> count;
        }
    };
    std::priority_queue<QueueEntry> q;
    constexpr ClassIndex kClasses = 10;
    for (ClassIndex i = 0; i < kClasses; i++) {
        q.push(QueueEntry{.count = CountType{0}, .class_index = i});
    }
    std::array<std::vector<YearType>, kClasses> class_to_year{};
    for (const auto [count, year] : value_counts) {
        const auto [_, inserted] = year_to_count.emplace(year, count);
        assert(inserted);

        const auto [min_count, index_of_class_with_min_count] = std::move(q.top());
        q.pop();
        class_to_year[index_of_class_with_min_count].push_back(year);
        q.push(
            QueueEntry{.count = min_count + count, .class_index = index_of_class_with_min_count});
    }

    for (ClassIndex class_index = 0; const auto& years_per_class : class_to_year) {
        CountType class_size = 0;
        for (const YearType year : years_per_class) {
            class_size += year_to_count.at(year);
        }
        std::cout << "Years for the class " << class_index << " (with size " << class_size
                  << "):\n";
        for (const YearType year : years_per_class) {
            std::cout << "    " << year << '\n';
        }

        class_index++;
    }
}
