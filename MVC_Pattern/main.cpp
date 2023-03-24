#include "Service.h"
#include <sstream>
using namespace alba_java;



enum UseCases
{
	INDI_SIGN_UP, INDI_SIGN_IN, COMP_SIGN_UP, COMP_SIGN_IN,
	BACK_TO_MENU, DELETE_INDI, DELETE_COMP, MODIFY_INDI, MODIFY_COMP, SIGN_OUT, EXIT,
	REGISTER_CV, MODIFY_CV, DELETE_CV, SEARCH_JP, SEARCH_JP_IN_RANGE, DETAIL_JP,
	REGISTER_JP, MODIFY_JP, DELETE_JP, SEARCH_CV, SEARCH_CV_IN_RANGE, DETAIL_CV,
	APPLY_CV, APPLY_INVALID_CV, APPLY_TO_INVALID_JP, SHOW_APPLIED_CV, MAX_USECASE_COUNT
};
#define  TESTCASE_COUNT 26
void ExcuteBlackBoxTest(unsigned int i)
{
	std::string use_cases[MAX_USECASE_COUNT];
	use_cases[INDI_SIGN_UP] = " 2 indi indi indi 1 Seok-hyun-young 010-6573-5181 2003.07.21 gugu030721@gmail.com 42013 Suseong-gu,Dongdaegue,72-11 ";
	use_cases[INDI_SIGN_IN] = " 1 indi indi ";
	use_cases[COMP_SIGN_UP] = " 2 comp comp comp 2 Kim-Naver-KaKao 053-1234-5678 zuzu123456@gmail.com 13456-67459 ";
	use_cases[COMP_SIGN_IN] = " 1 comp comp ";
	use_cases[BACK_TO_MENU] = " 2 ";
	use_cases[SIGN_OUT] = " 6";
	use_cases[MODIFY_INDI] = " 0 indi 1 Seok-young-hyun ";
	use_cases[MODIFY_COMP] = " 0 comp 1 Kim-NaKa ";
	use_cases[DELETE_INDI] = " 7 indi ";
	use_cases[DELETE_COMP] = " 7 comp ";
	use_cases[EXIT] = " 3";
	use_cases[REGISTER_CV] = " 2 Seok-hyun-young 20 Transgender Daegu KNU BulKot-Chung-Nyun My,First,CV.. ";
	use_cases[MODIFY_CV] = " 3 n 0 1 Seok-young-hyun ";
	use_cases[DELETE_CV] = " 4 n 0 ";
	use_cases[SEARCH_CV] = " 1 0 My,First ";
	use_cases[SEARCH_CV_IN_RANGE] = " 1 6 19 21 ";
	use_cases[DETAIL_CV] = " 0 exit_to_menu ";
	use_cases[APPLY_CV] = " 5 0 0 ";
	use_cases[APPLY_INVALID_CV] = " 5 0 100 ";
	use_cases[APPLY_TO_INVALID_JP] = " 5 100 ";
	use_cases[SHOW_APPLIED_CV] = " 5 ";
	use_cases[REGISTER_JP] = " 2 SODA-LAB Researcher Daegu Alba Gajok-Gateun-Bunwigi 12 365 4 5 12 20 Please,comeon! ";
	use_cases[MODIFY_JP] = " 3 n 0 1 DongAndSunLAB ";
	use_cases[DELETE_JP] = " 4 n 0 ";
	use_cases[SEARCH_JP] = " 1 1 Please ";
	use_cases[SEARCH_JP_IN_RANGE] = " 1 6 10 14 ";
	use_cases[DETAIL_JP] = " 0 exit_to_menu ";

	std::string testcases[TESTCASE_COUNT];

	//S1
	testcases[0] = use_cases[INDI_SIGN_UP] + use_cases[EXIT];
	testcases[1] = use_cases[COMP_SIGN_UP] + use_cases[EXIT];

	//S2
	testcases[2] = use_cases[INDI_SIGN_UP] + use_cases[INDI_SIGN_IN] + use_cases[SIGN_OUT] + use_cases[EXIT];
	testcases[3] = use_cases[COMP_SIGN_UP] + use_cases[COMP_SIGN_IN] + use_cases[SIGN_OUT] + use_cases[EXIT];
	testcases[4] = use_cases[INDI_SIGN_IN] + use_cases[BACK_TO_MENU] + use_cases[EXIT];

	//S3
	testcases[5] = use_cases[INDI_SIGN_UP] + use_cases[INDI_SIGN_IN] + use_cases[SIGN_OUT] + use_cases[INDI_SIGN_IN] + use_cases[SIGN_OUT] + use_cases[EXIT];

	//S4
	testcases[6] = use_cases[COMP_SIGN_UP] + use_cases[COMP_SIGN_IN] + use_cases[DELETE_COMP] + use_cases[EXIT];

	//S5
	testcases[7] = use_cases[INDI_SIGN_UP] + use_cases[INDI_SIGN_IN] + use_cases[SEARCH_JP] + use_cases[SIGN_OUT] + use_cases[EXIT];
	testcases[8] = use_cases[COMP_SIGN_UP] + use_cases[COMP_SIGN_IN] + use_cases[REGISTER_JP] + use_cases[SIGN_OUT]
		+ use_cases[INDI_SIGN_UP] + use_cases[INDI_SIGN_IN] + use_cases[SEARCH_JP] + use_cases[DETAIL_JP] + use_cases[SIGN_OUT] + use_cases[EXIT];
	testcases[9] = use_cases[COMP_SIGN_UP] + use_cases[COMP_SIGN_IN] + use_cases[REGISTER_JP] + use_cases[SIGN_OUT]
		+ use_cases[INDI_SIGN_UP] + use_cases[INDI_SIGN_IN] + use_cases[SEARCH_JP_IN_RANGE] + use_cases[DETAIL_JP] + use_cases[SIGN_OUT] + use_cases[EXIT];
	//S6
	testcases[10] = use_cases[INDI_SIGN_UP] + use_cases[INDI_SIGN_IN] + use_cases[REGISTER_CV] + use_cases[SIGN_OUT] + use_cases[EXIT];
	testcases[11] = use_cases[INDI_SIGN_UP] + use_cases[INDI_SIGN_IN] + use_cases[REGISTER_CV] + use_cases[MODIFY_CV] + use_cases[SIGN_OUT] + use_cases[EXIT];
	testcases[12] = use_cases[INDI_SIGN_UP] + use_cases[INDI_SIGN_IN] + use_cases[REGISTER_CV] + use_cases[DELETE_CV] + use_cases[SIGN_OUT] + use_cases[EXIT];

	//S7
	testcases[13] = use_cases[COMP_SIGN_UP] + use_cases[COMP_SIGN_IN] + use_cases[REGISTER_JP] + use_cases[SIGN_OUT]
		+ use_cases[INDI_SIGN_UP] + use_cases[INDI_SIGN_IN] + use_cases[REGISTER_CV] + use_cases[APPLY_CV] + use_cases[SIGN_OUT] + use_cases[EXIT];
	testcases[14] = use_cases[COMP_SIGN_UP] + use_cases[COMP_SIGN_IN] + use_cases[REGISTER_JP] + use_cases[SIGN_OUT]
		+ use_cases[INDI_SIGN_UP] + use_cases[INDI_SIGN_IN] + use_cases[REGISTER_CV] + use_cases[APPLY_INVALID_CV] + use_cases[SIGN_OUT] + use_cases[EXIT];
	testcases[15] = use_cases[COMP_SIGN_UP] + use_cases[COMP_SIGN_IN] + use_cases[REGISTER_JP] + use_cases[SIGN_OUT]
		+ use_cases[INDI_SIGN_UP] + use_cases[INDI_SIGN_IN] + use_cases[REGISTER_CV] + use_cases[APPLY_TO_INVALID_JP] + use_cases[SIGN_OUT] + use_cases[EXIT];

	//S8
	testcases[16] = use_cases[COMP_SIGN_UP] + use_cases[COMP_SIGN_IN] + use_cases[MODIFY_COMP] + use_cases[SIGN_OUT] + use_cases[EXIT];
	testcases[17] = use_cases[INDI_SIGN_UP] + use_cases[INDI_SIGN_IN] + use_cases[MODIFY_INDI] + use_cases[SIGN_OUT] + use_cases[EXIT];

	//S9
	testcases[18] = use_cases[COMP_SIGN_UP] + use_cases[COMP_SIGN_IN] + use_cases[REGISTER_JP] + use_cases[SIGN_OUT] + use_cases[EXIT];
	testcases[19] = use_cases[COMP_SIGN_UP] + use_cases[COMP_SIGN_IN] + use_cases[REGISTER_JP] + use_cases[MODIFY_JP] + use_cases[SIGN_OUT] + use_cases[EXIT];
	testcases[20] = use_cases[COMP_SIGN_UP] + use_cases[COMP_SIGN_IN] + use_cases[REGISTER_JP] + use_cases[DELETE_JP] + use_cases[SIGN_OUT] + use_cases[EXIT];

	//S10
	testcases[21] = use_cases[COMP_SIGN_UP] + use_cases[COMP_SIGN_IN] + use_cases[SEARCH_CV] + use_cases[SIGN_OUT] + use_cases[EXIT];
	testcases[22] = use_cases[INDI_SIGN_UP] + use_cases[INDI_SIGN_IN] + use_cases[REGISTER_CV] + use_cases[SIGN_OUT]
		+ use_cases[COMP_SIGN_UP] + use_cases[COMP_SIGN_IN] + use_cases[SEARCH_CV] + use_cases[DETAIL_CV] + use_cases[SIGN_OUT] + use_cases[EXIT];
	testcases[23] = use_cases[INDI_SIGN_UP] + use_cases[INDI_SIGN_IN] + use_cases[REGISTER_CV] + use_cases[SIGN_OUT]
		+ use_cases[COMP_SIGN_UP] + use_cases[COMP_SIGN_IN] + use_cases[SEARCH_CV_IN_RANGE] + use_cases[DETAIL_CV] + use_cases[SIGN_OUT] + use_cases[EXIT];
	//S11
	testcases[24] = use_cases[COMP_SIGN_UP] + use_cases[COMP_SIGN_IN] + use_cases[REGISTER_JP] + use_cases[SIGN_OUT]
		+ use_cases[INDI_SIGN_UP] + use_cases[INDI_SIGN_IN] + use_cases[REGISTER_CV] + use_cases[APPLY_CV] + use_cases[SIGN_OUT]
		+ use_cases[COMP_SIGN_IN] + use_cases[SHOW_APPLIED_CV] + use_cases[DETAIL_CV] + use_cases[SIGN_OUT] + use_cases[EXIT];
	testcases[25] = use_cases[COMP_SIGN_UP] + use_cases[COMP_SIGN_IN] + use_cases[SHOW_APPLIED_CV] + use_cases[SIGN_OUT] + use_cases[EXIT];

	std::stringstream simulator;
	simulator.str(testcases[i]);
	Service test_service(std::cout, simulator);
	test_service.Run();
}

int main(void)
{

	Service a(std::cout, std::cin);
	a.Run();
	return 0;
}