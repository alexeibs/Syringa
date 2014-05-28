#include <iostream>
#include <syringa/syringa.h>
#include "simplereport.h"

using namespace std;
using namespace Syringa;
using Tests::SimpleReport;

string testCase1()
{
    run([](){
        describe("Group 1", []() {
            it("Test 1", []() {
                expect(2 * 2).toEqual(5);
            });
        });
    });
    return
R"report(
---
Group 1 - Test 1

Expected 4 to equal 5

)report";
}

string testCase2()
{
    run([](){
        describe("Group 1", []() {
            it("Test 1", []() {
                expect(2 * 2).toEqual(4);
                describe("Subgroup", [](){});
                it("Subgroup", [](){});
            });
            it("Test 2", []() {
                expect(2 * 2).toEqual(5);
            });
            it("Test 3", []() {
                expect(2 * 2).toEqual(6);
            });
            describe("Subgroup 1", []() {
                it("Test 4", []() {
                    expect(1).toEqual(2);
                });
            });
        });
    });
    return
R"report(
---
Group 1 - Test 1

Syntax error - unexpected "describe"
Syntax error - unexpected "it"


---
Group 1 - Test 2

Expected 4 to equal 5


---
Group 1 - Test 3

Expected 4 to equal 6


---
Group 1 - Subgroup 1 - Test 4

Expected 1 to equal 2

)report";
}

int main()
{
    Tests::SimpleReport report;
    setOutput([&report](const std::list<std::string>& callStack, const std::string& messages) {
        report.write(callStack, messages);
    });
    auto test1_expected = testCase1();
    auto test1_result = report.content();
    report.clear();
    auto test2_expected = testCase2();
    auto test2_result = report.content();

    report.clear();
    run([&](){
        describe("Syringa tests", [&]() {
            it("Test 1", [&]() {
                expect(test1_expected == test1_result).toEqual(true);
            });
            it("Test 2", [&]() {
                expect(test2_expected == test2_result).toEqual(true);
            });
        });
    });

    if (report.content().empty())
        cout << "All tests have been passed";
    else
        cout << report.content();

    return 0;
}
