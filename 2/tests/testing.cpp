#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>
#include <sstream>
#include "signals/binarySignal.h"
#include "signals/signalStatus.h"

using namespace signals;

TEST_CASE("signalStatus") {
    SECTION("constructor") {
        SignalStatus a;
        REQUIRE(a.getStatus() == false);
        REQUIRE(a.getTime() == 1);

        a = SignalStatus(true, 3);
        REQUIRE(a.getStatus() == true);
        REQUIRE(a.getTime() == 3);

        std::string b = "1111100111";
        a = SignalStatus(b.begin(), b.end());
        REQUIRE(a.getStatus() == true);
        REQUIRE(a.getTime() == 5);

        b = "000";
        a = SignalStatus(b.end(), b.end());
        REQUIRE(a.getStatus() == false);
        REQUIRE(a.getTime() == 1);

        std::vector<int> c = {0, 0, 1};
        a = SignalStatus(c.begin(), c.end());
        REQUIRE(a.getStatus() == false);
        REQUIRE(a.getTime() == 2);

        char d[11] = "1110000000";
        a = SignalStatus(d, d + 10);
        REQUIRE(a.getStatus() == true);
        REQUIRE(a.getTime() == 3);
    }

    SECTION("function") {
        SignalStatus a = SignalStatus(true, 5);
        a.setStatus(false);
        REQUIRE(a.getStatus() == false);
        a.setTime(4);
        REQUIRE(a.getTime() == 4);

        a.changeTime(-3);
        REQUIRE(a.getTime() == 1);
        a.changeTime(6);
        REQUIRE(a.getTime() == 7);
        a.changeTime(-8);
        REQUIRE(a.getTime() == 1);
/*        a.changeTime(LONG_MAX);
        REQUIRE(a.getTime() == (unsigned long int)LONG_MAX + 1);
        a.changeTime(LONG_MAX);
        REQUIRE(a.getTime() == UINT32_MAX);
        a.changeTime(1);
        REQUIRE(a.getTime() == 1);
        a.changeTime(LONG_MAX);
        a.changeTime(LONG_MIN);
        REQUIRE(a.getTime() == 1);
        a.changeTime(LONG_MAX);
        a.changeTime(1);
        REQUIRE(a.getTime() == (unsigned long int)LONG_MAX + 2);
        a.changeTime(LONG_MIN);
        REQUIRE(a.getTime() == 1);
        a.changeTime(LONG_MAX);
        a.changeTime(2);
        REQUIRE(a.getTime() == (unsigned long int)LONG_MAX + 3);
        a.changeTime(LONG_MIN);
        REQUIRE(a.getTime() == 2);
*/
    
        REQUIRE(formatStatus(a) == "_");
    }

    SECTION("operator") {
        SignalStatus a = SignalStatus(false, 9);
        REQUIRE(bool(a) == false);
        REQUIRE(bool(~a) == true);

        std::stringstream out, in;
        out << a;
        REQUIRE(out.str() == "Статус сигнала: 0 | Длительность: 9\n");

        in << "1 121\n";
        in >> a;
        REQUIRE(a.getStatus() == true);
        REQUIRE(a.getTime() == 121);
    }
}


TEST_CASE("binarySignal") {
    SECTION("constructor") {
        BinarySignal a;
        REQUIRE(a.getSize() == 0);
        REQUIRE(a.getCapacity() == 8);
        REQUIRE_THROWS(a[0]);

        a = BinarySignal(10);
        REQUIRE(a.getSize() == 0);
        REQUIRE(a.getCapacity() == 10);
        REQUIRE_THROWS(a[9]);

        a = BinarySignal(SignalStatus(true, 3));
        REQUIRE(a.getSize() == 1);
        REQUIRE(a[2] == true);

        a = BinarySignal(true, 2);
        REQUIRE(a.getSize() == 1);
        REQUIRE(a[1] == true);

        std::string b = "11111000111010101";
        a = BinarySignal(b.begin(), b.end());
        REQUIRE(a.getSize() == 9);
        REQUIRE(a.getCapacity() == 16);
        REQUIRE(a[0] == true);
        REQUIRE(a[4] == true);
        REQUIRE(a[5] == false);

        std::vector c = {1, 1, 0, 0};
        a = BinarySignal(c.begin(), c.end());
        REQUIRE(a.getSize() == 2);
        REQUIRE(a.getCapacity() == 8);
        REQUIRE(a[3] == false);

        BinarySignal d = BinarySignal(a);
        REQUIRE(d.getSize() == 2);
        REQUIRE(d.getCapacity() == 8);
        REQUIRE(d[1] == true);

        BinarySignal e = BinarySignal(std::move(d));
        REQUIRE(d.getSize() == 0);
        REQUIRE(e.getCapacity() == 8);
        REQUIRE_THROWS(d[1]);
        REQUIRE(e[3] == false);
    }

    SECTION("function") {
        std::string b = "011100";
        BinarySignal a = BinarySignal(b.begin(), b.end());
        a.invert(); // 100011
        REQUIRE(a[0] == true);
        REQUIRE(a[3] == false);

        std::string d = "0011";
        BinarySignal c = BinarySignal(d.begin(), d.end());
        a.insert(c, 2); // 10 0011 0011
        REQUIRE(a.getSize() == 6);
        REQUIRE(a.getCapacity() == 8);
        REQUIRE(a[0] == true);
        REQUIRE(a[1] == false);
        REQUIRE(a[2] == false);
        REQUIRE(a[3] == false);
        REQUIRE(a[4] == true);
        REQUIRE(a[5] == true);
        REQUIRE(a[6] == false);
        REQUIRE(a[7] == false);
        REQUIRE(a[8] == true);
        REQUIRE(a[9] == true);
        REQUIRE_THROWS(a[10]);

        a.insert(c, 0); // 0011 10 0011 0011
        REQUIRE(a.getSize() == 8);
        REQUIRE(a.getCapacity() == 16);
        REQUIRE(a[0] == false);
        REQUIRE(a[3] == true);
        REQUIRE(a[4] == true);

        a.deleteFragment(3, 5); // 00 1 11 00 11
        REQUIRE(a.getSize() == 5);
        REQUIRE(a[0] == false);
        REQUIRE(a[1] == false);
        REQUIRE(a[2] == true);
        REQUIRE(a[3] == true);
        REQUIRE(a[4] == true);
        REQUIRE(a[5] == false);
        REQUIRE(a[6] == false);
        REQUIRE(a[7] == true);
        REQUIRE(a[8] == true);
        REQUIRE_THROWS(a[9]);

        a.deleteFragment(5, 1); // 00 1 11 0 11
        REQUIRE(a[4] == true);
        REQUIRE(a[5] == false);
        REQUIRE(a[6] == true);

        a.deleteFragment(0, 0); // 00 1 11 0 11
        REQUIRE(a[0] == false);
        REQUIRE(a[1] == false);
        
        d = "00000";
        c = BinarySignal(d.begin(), d.end());
        a.insert(c, 0); // 00000 00 1 11 0 11
        a.deleteFragment(1, 3); // 00 00 1 11 0 11
        REQUIRE(a.getSize() == 6);
        REQUIRE(a[0] == false);
        REQUIRE(a[1] == false);
        REQUIRE(a[2] == false);
        REQUIRE(a[9] == true);
        REQUIRE_THROWS(a[10]);

        a.deleteFragment(0, UINT32_MAX); // -
        REQUIRE(a.getSize() == 0);
        REQUIRE_THROWS(a[0]);
    }

    SECTION("operator") {
        std::string b = "00111001011011100";
        BinarySignal a = BinarySignal(b.begin(), b.end());
        BinarySignal c = a;
        REQUIRE(c.getSize() == 9);
        REQUIRE(c.getCapacity() == 16);
        REQUIRE(c[4] == true);
        REQUIRE(c[15] == false);
        REQUIRE_THROWS(c[17]);
    
        BinarySignal d = std::move(c);
        REQUIRE(d.getSize() == 9);
        REQUIRE(c.getCapacity() == 0);
        REQUIRE(d[4] == true);
        REQUIRE_THROWS(c[15]);

        REQUIRE(!bool(c));
        REQUIRE(bool(d));

        a += d;
        REQUIRE(a.getSize() == 18);
        REQUIRE(a.getCapacity() == 32);
        REQUIRE(a[4] == true);
        REQUIRE(a[17] == false);
        REQUIRE(a[27] == true);
        REQUIRE_THROWS(a[34]);

        BinarySignal e = d*(unsigned int)3;
        REQUIRE(e.getSize() == 27);
        REQUIRE(e.getCapacity() == 48);
        REQUIRE(e[15] == false);
        REQUIRE(e[17] == false);
        REQUIRE(e[19] == true);
        REQUIRE(e[34] == false);
        REQUIRE(e[37] == true);
        REQUIRE_THROWS(e[56]);


        b = "00111001";
        a = BinarySignal(b.begin(), b.end());
        std::stringstream out, in;
        out << a;
        REQUIRE(out.str() == "__/###\\__/#");

        BinarySignal f;
        in << "2 1 30 0 20\n ";
        in >> f;
        REQUIRE(f.getSize() == 2);
        REQUIRE(f.getCapacity() == 8);
        REQUIRE(f[29] == true);
        REQUIRE(f[30] == false);
        REQUIRE_THROWS(f[50]);
    }
}
