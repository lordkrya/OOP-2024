#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>

#include <vector>
#include <string>
#include <limits>

#include "orderedTable.hpp"
#include "OTableIterator.hpp"

using namespace table;

TEST_CASE("Elem") {
    SECTION("Constructor") {
        Elem<char, std::string> pair1;
        REQUIRE(pair1.getKey() == 0);
        REQUIRE(pair1.getVal().empty());

        std::string str1 = "ABOBA";
        std::string str2 = "Key";
        Elem<std::string, std::string> pair2(str1, str2);
        REQUIRE(pair2.getKey() == str1);
        REQUIRE(pair2.getVal() == str2);

        Elem<std::string, std::string> pair3(std::move(str1), std::move(str2));
        REQUIRE(pair3.getKey() != str1);
        REQUIRE(pair3.getVal() != str2);
        REQUIRE(pair2.getKey() == pair3.getKey());
        REQUIRE(pair2.getVal() == pair3.getVal());

        Elem<std::string, std::string> pair4(pair3);
        REQUIRE(pair4.getKey() == pair3.getKey());
        REQUIRE(pair4.getVal() == pair3.getVal());

        Elem<std::string, std::string> pair5(std::move(pair4));
        REQUIRE(pair5.getKey() != pair4.getKey());
        REQUIRE(pair5.getVal() != pair4.getVal());
        REQUIRE(pair5.getVal() == pair3.getVal());

        swap(pair4, pair5);
        REQUIRE(pair5.getVal() != pair4.getVal());
        REQUIRE(pair4.getVal() == pair3.getVal());

        pair5 = std::move(pair3);
        REQUIRE(pair5.getVal() == pair4.getVal());

        pair3 = pair5;
        REQUIRE(pair5.getVal() == pair3.getVal());
    }
    SECTION("Function") {
        Elem<int, std::string> pair1(1, "WELL_DONE");
        REQUIRE(pair1.getKey() == pair1.getKey());
        REQUIRE(pair1.getVal() == pair1.getVal());
    }
    SECTION("Operator") {
        Elem<float, std::string> pair1(1.2, "GOOOL!");
        Elem<float, std::string> pair2(2.4, "GOOOL!");
        Elem<float, std::string> pair3(2.4, "GOL!");
        REQUIRE(pair1 != pair2);
        REQUIRE(pair1 < pair2);
        REQUIRE(pair2 == pair3);
        REQUIRE(pair2 == 2.4);
        REQUIRE(pair2 < 3.6);
    }
}

TEST_CASE("OrderedTable") {
    SECTION("Constructor") {
        OrderedTable<int, std::string> table1;
        REQUIRE(table1.empty());

        Elem<int, std::string> pair1(5, "GOIDA!");
        OrderedTable<int, std::string> table2(2, pair1);
        REQUIRE(table2.size() == 2);
        REQUIRE(table2[0].getKey() == 5);
        REQUIRE(table2[1].getVal() == "GOIDA!");
        

        std::vector<Elem<int, std::string>> arr {
            Elem<int, std::string>(1, "NACHT!"), 
            Elem<int, std::string>(7, "Ferdinand"),
            Elem<int, std::string>(9, "AUF"),
        };
        OrderedTable<int, std::string> table3(arr.begin(), arr.end());
        REQUIRE(table3.size() == 3);
        REQUIRE(table3[0].getKey() == 1);
        REQUIRE(table3[1].getVal() == "Ferdinand");
        REQUIRE(table3[2].getKey() == 9);

        OrderedTable<int, std::string> table4({Elem<int, std::string>(8, "Il")});
        REQUIRE(table4.size() == 1);
        REQUIRE(table4[0].getVal() == "Il");
        
        OrderedTable<int, std::string> table5(std::move(table2));
        REQUIRE(table5.size() == 2);
        REQUIRE(table5[1].getKey() == 5);
        REQUIRE(table5[0].getVal() == "GOIDA!");

        OrderedTable<int, std::string> table6(table5);
        REQUIRE(table6.size() == 2);
        REQUIRE(table6[1].getKey() == table5[1].getKey());
        REQUIRE(table6[0].getVal() == table5[0].getVal());

        table2 = std::move(table3);
        REQUIRE(table2.size() == 3);
        REQUIRE(table2[0].getKey() == 1);
        REQUIRE(table2[1].getVal() == "Ferdinand");
        REQUIRE(table2[2].getKey() == 9);

        table3 = table2;
        REQUIRE(table3.size() == 3);
        REQUIRE(table3[0].getVal() == table2[0].getVal());
        REQUIRE(table3[1].getKey() == table2[1].getKey());
        REQUIRE(table3[2].getVal() == table3[2].getVal());

        table4 = {Elem<int, std::string>(8, "Il"), Elem<int, std::string>(8, "Il")};
        REQUIRE(table4.size() == 2);
        REQUIRE(table4[0].getVal() == "Il");
        REQUIRE(table4[1].getKey() == 8);

        swap(table2, table1);
        REQUIRE(table2.empty());
        REQUIRE(table1[0].getVal() == "NACHT!");
        REQUIRE(table1[1].getKey() == 7);
        REQUIRE(table1[2].getVal() == "AUF");
    }
    SECTION("Function") {
        std::vector<Elem<int, std::string>> arr {
            Elem<int, std::string>(1, "NACHT!"),
            Elem<int, std::string>(7, "Ferdinand"),
            Elem<int, std::string>(7, "AUF"),
        };
        OrderedTable<int, std::string> table1(arr.begin(), arr.end());
        REQUIRE(table1.key_comp()(1, 2) == true);
        REQUIRE(table1.key_comp()(3, 3) == false);
        REQUIRE(table1.value_comp()(arr[0], arr[1]) == true);
        REQUIRE(table1.value_comp()(arr[1], arr[2]) == false);
        REQUIRE(table1.size() == 3);
        REQUIRE(table1.max_size() == std::numeric_limits<size_t>::max());
        REQUIRE(table1.empty() == false);

        REQUIRE(*(table1.begin()) == table1[0]);
        REQUIRE(*(table1.end() - 1) == table1[2]);

        table1.emplace_hint(table1.end(), 9, "goi");
        REQUIRE(table1[3].getKey() == 9);
        table1.emplace_hint(table1.cend(), 2, "neGoi");
        REQUIRE(table1[1].getVal() == "neGoi");
        table1.emplace(4, "imagen");
        REQUIRE(table1[2].getKey() == 4);
        table1.insert(table1.end() - 1, Elem<int, std::string>(12, "Henschel"));
        REQUIRE(table1[6].getKey() == 12);
        table1.insert(Elem<int, std::string>(3, "dubl"));
        REQUIRE(table1[2].getVal() == "dubl");

        Elem<int, std::string> dublDubl(15, "Sbk");
        table1.insert(OTableIterator(table1.end()), dublDubl);
        table1.insert(dublDubl);
        REQUIRE(table1[8].getKey() == 15);
        REQUIRE(table1[9].getVal() == "Sbk");

        table1.insert(table1.end(), 2, Elem<int, std::string>(17, "ardens"));
        REQUIRE(table1[10].getKey() == 17);
        REQUIRE(table1[11].getVal() == "ardens");
        table1.insert(1, Elem<int, std::string>(16, "claus"));
        table1.insert(0, Elem<int, std::string>(16, "claus"));
        REQUIRE(table1[10].getKey() == 16);

        table1.insert(arr.begin(), arr.end() - 1);
        table1.insert(arr.begin(), arr.begin());
        REQUIRE(table1[0].getVal() == "NACHT!");
        REQUIRE(table1[5].getKey() == 7);

        table1.insert({
            Elem<int, std::string>(25, "I`ve"), 
            Elem<int, std::string>(50, "tired"),
        });
        REQUIRE(table1[15].getVal() == "I`ve");
        REQUIRE(table1[16].getKey() == 50);

        OrderedTable<int, std::string> table2(2, Elem<int, std::string>(26, "GOIDA!"));
        table1.merge(table2);
        REQUIRE(table1[16].getVal() == "GOIDA!");
        REQUIRE(table1[17].getKey() == 26);
        REQUIRE(table2[0].getVal() != table1[16].getVal());
        REQUIRE(table2[1].getKey() == 26);

        REQUIRE(table1.find(4) == table1.begin() + 4);
        REQUIRE(table1.lower_bound(7) == table1.begin() + 5);
        REQUIRE(table1.upper_bound(7) == table1.begin() + 8);
        REQUIRE(table1.equal_range(26) == std::pair
        <OrderedTable<int, std::string>::iterator, OrderedTable<int, std::string>::iterator>
        (table1.begin() + 16, table1.begin() + 18));
        REQUIRE(table1.count(26) == 2);

        REQUIRE(table1.contains(100) == false);
        REQUIRE(table1.contains(12));

        table1.erase(table1.begin() + 1);
        REQUIRE(table1[1].getKey() == 2);
        table1.erase(table1.begin(), table1.begin() + 3);
        REQUIRE(table1[0].getVal() == "imagen");

        table1.clear();
        REQUIRE(table1.empty());

        table1.assign(arr.begin(), arr.end());
        REQUIRE(table1.size() == 3);
        REQUIRE(table1[0].getKey() == 1);
        REQUIRE(table1[1].getVal() == "Ferdinand");
        REQUIRE(table1[2].getKey() == 7);

        table1.assign({
            Elem<int, std::string>(25, "I`ve"),
            Elem<int, std::string>(50, "tired"),
        });
        REQUIRE(table1.size() == 2);
        REQUIRE(table1[0].getVal() == "I`ve");
        REQUIRE(table1[1].getKey() == 50);

        table1.assign(2, Elem<int, std::string>(26, "GOIDA!"));
        REQUIRE(table1.size() == 2);
        REQUIRE(table1[0].getVal() == "GOIDA!");
        REQUIRE(table1[1].getKey() == 26);
    }
    SECTION("Operator") {
        OrderedTable<int, std::string> table1(2, Elem<int, std::string>(5, "GOIDA!"));

        Elem<int, std::string> pair1(5, "GOIDA!");
        OrderedTable<int, std::string> table2(2, pair1);

        std::vector<Elem<int, std::string>> arr {
            Elem<int, std::string>(5, "GOIDA!"), 
            Elem<int, std::string>(7, "Ferdinand"),
            Elem<int, std::string>(9, "AUF"),
        };
        OrderedTable<int, std::string> table3(arr.begin(), arr.end());

        REQUIRE((table1 == table2));
        REQUIRE((table2 != table3));
        REQUIRE(table1[0] == table2[1]);
        REQUIRE(table2[1] != table3[2]);
    }
}
