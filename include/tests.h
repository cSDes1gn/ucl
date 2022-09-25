/**
 * @file tests.h
 * @author ztnel (christian911@sympatico.ca)
 * @brief test wrapper definitions
 * @version 0.1
 * @date 2022-09
 * 
 * @copyright Copyright © 2022 Christian Sargusingh
 * 
 */

typedef int test_status_t;

#define TEST_SUCCESS (test_status_t)0
#define TEST_FAILURE (test_status_t)1

typedef test_status_t (*test_func)(void);

void test_run(char *name, test_func test);
test_status_t test_eeprom(void);
test_status_t test_pwm(void);