#define CATCH_CONFIG_MAIN

#include <vector>
#include <thesoup/types/types.hpp>
#include <catch2/catch.hpp>
#include <stdexcept>

using thesoup::types::Result;
using thesoup::types::Slice;
using thesoup::types::IsForwardIteratorOfType;

SCENARIO("Results test") {

    GIVEN("I have an OK result") {

        auto res {Result<int, int>::success(10)};

        WHEN("I attempt to access it's fields") {

            THEN("Everything should be as expected") {

                REQUIRE(res);
                REQUIRE(10 == res.unwrap());
                REQUIRE_THROWS_AS(res.error(), std::runtime_error);
            }
        }
    }

    GIVEN("I have an error result") {

        auto res {Result<int, int>::failure(2)};

        WHEN("I attempt to access it's fields") {

            THEN("Everything should be as expected") {

                REQUIRE(!res);
                REQUIRE(2 == res.error());
                REQUIRE_THROWS_AS(res.unwrap(), std::runtime_error);
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