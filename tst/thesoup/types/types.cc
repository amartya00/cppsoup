#define CATCH_CONFIG_MAIN

#include <vector>
#include <thesoup/types/types.hpp>
#include <catch2/catch.hpp>
#include <exception>

using thesoup::types::Result;
using thesoup::types::error;
using thesoup::types::ok;
using thesoup::types::OperationType;
using thesoup::types::Slice;
using thesoup::types::IsForwardIteratorOfType;

SCENARIO("Results test") {

    GIVEN("I have an OK result") {

        auto res {ok<int, int>(10)};

        WHEN("I attempt to access it's fields") {

            auto type {res.type};
            auto val {res.val};
            auto err {res.error};
            auto msg {res.message};

            THEN("Everything should be as expected") {

                REQUIRE(OperationType::OK == type);
                REQUIRE(10 == *res.val);
                REQUIRE(std::nullopt == res.error);
                REQUIRE(std::nullopt == res.message);
            }
        }
    }

    GIVEN("I have an error result") {

        auto res {error<int, int>(2)};

        WHEN("I attempt to access it's fields") {

            auto type {res.type};
            auto val {res.val};
            auto err {res.error};
            auto msg {res.message};

            THEN("Everything should be as expected") {

                REQUIRE(OperationType::ERR == type);
                REQUIRE(std::nullopt == res.val);
                REQUIRE(2 == *res.error);
                REQUIRE(std::nullopt == res.message);
            }
        }
    }

    GIVEN("I have an error result with a message") {

        auto res {error<int, int>(2, "Error message")};

        WHEN("I attempt to access it's fields") {

            auto type {res.type};
            auto val {res.val};
            auto err {res.error};
            auto msg {res.message};

            THEN("Everything should be as expected") {

                REQUIRE(OperationType::ERR == type);
                REQUIRE(std::nullopt == res.val);
                REQUIRE(2 == *res.error);
                REQUIRE(std::string("Error message") == *res.message);
            }
        }
    }
}

SCENARIO("Slice test happy case") {

    GIVEN("I have a slice") {

        std::vector<int> test_arr {1,2,3,4,5};
        std::vector<int> reference_arr {1,2,3,4,5};
        Slice<int> test_slice {
            &test_arr[0],
            test_arr.size()
        };

        WHEN("I access elements") {

            int test1 {test_slice[0]};
            int test2 {test_slice[4]};

            THEN("I should be able to do so") {

                REQUIRE(1 == test1);
                REQUIRE(5 == test2);
            }
        }

        WHEN("I Iterate through elements in read only mode") {

            THEN("I should be able to do so") {

                std::size_t idx {0};
                for (const int& elem : test_slice) {

                    REQUIRE(reference_arr[idx] == elem);
                    idx++;
                }
            }
        }

        WHEN("I Iterate through th elements in a R/W mode") {

            THEN("I should be able to do so") {

                std::size_t idx {0};
                for (auto& item : test_slice) {

                    item++;
                    REQUIRE(reference_arr[idx]+1 == item);
                    idx++;
                }
            }
        }
    }
}

SCENARIO("Slice test exception case") {

    GIVEN("I have a slice") {

        std::vector<int> test_arr {1,2,3,4,5};
        std::vector<int> reference_arr {1,2,3,4,5};
        Slice<int> test_slice {
            &test_arr[0],
            test_arr.size()
        };

        WHEN("I access an out of range element") {

            THEN("It should throw an exception") {

                REQUIRE_THROWS_AS(test_slice[10], std::out_of_range);
                REQUIRE_THROWS_AS(test_slice[-10], std::out_of_range);
            }
        }
    }
}

SCENARIO("Forward iterator test") {

    GIVEN("I have a forward iterator") {

        std::vector<int> v1 {1, 2, 3, 4, 5};

        WHEN("I create a template to test it's type") {

            THEN("It should work") {

                REQUIRE(IsForwardIteratorOfType<decltype(v1.begin()), int>::value);
            }

            AND_WHEN("I test it for something that is not a forward iterator") {

                THEN("It should fail") {

                    REQUIRE_FALSE(IsForwardIteratorOfType<int, int>::value);
                }
            }
        }
    }
}