#include <glib.h>
#include "cal.h"

void test_month()
{
	int day = 1;
	g_assert_cmpint(cal(1, day), ==, 1);
	g_assert_cmpint(cal(12, day), ==, 1);
	g_assert_cmpint(cal(13, day), ==, 0);
}

void test_day()
{
	int month = 2;
	g_assert_cmpint(cal(month, 1), ==, 1);
	g_assert_cmpint(cal(month, 29), ==, 1);
	g_assert_cmpint(cal(month, 30), ==, 0);
}

int main(int argc, char* argv[])
{
	/* 初始化測試框架，必須傳入NULL做為結束 */
	g_test_init(&argc, &argv, NULL);

	/* 註冊測試函數，在測試過程中，這些函數將按順序執行 */
	g_test_add_func("/cal/month", test_month);
	g_test_add_func("/cal/day", test_day);

	/* 運行測試 */
	return g_test_run();
}
