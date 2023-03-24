#include "Service.h"

namespace alba_java
{

	void Service::Run()
	{
		LOG_ALBA_JAVA_METHOD("Run");
		unsigned int input;
		while (mSessionUserId.empty())
		{
			mCLI.DisplayStartPage();
			mCLI.GetNumberInputSafely(input, 1, 3);
			switch (input)
			{
			case 1:
				signIn();
				break;
			case 2:
				signUp();
				break;
			default:
				return;
			}
			mCLI.ClearScreen();
		}
		Update();
	}

	void Service::Update()
	{
		LOG_ALBA_JAVA_METHOD("Update");
		unsigned int input;
		while (!mSessionUserId.empty())
		{
			if (mIsSessionUserCompany)
			{
				mCLI.DisplayCompanyMenu();
				mCLI.GetNumberInputSafely(input, 0, 7);
				mCLI.ClearScreen();
				switch (input)
				{
				case 0:
					updateCompany();
					break;
				case 1:
					searchCV();
					break;
				case 2:
					registerJP();
					break;
				case 3:
					updateJP();
					break;
				case 4:
					deleteJP();
					break;
				case 5:
					showAppliedCVs();
					break;
				case 6:
					signOut();
					break;
				default:
					deleteAccountAndSignOut();
					break;
				}
			}
			else
			{
				mCLI.DisplayIndividualMenu();
				mCLI.GetNumberInputSafely(input, 0, 7);
				mCLI.ClearScreen();
				switch (input)
				{
				case 0:
					updateIndividual();
					break;
				case 1:
					searchJP();
					break;
				case 2:
					registerCV();
					break;
				case 3:
					updateCV();
					break;
				case 4:
					deleteCV();
					break;
				case 5:
					applyCV();
					break;
				case 6:
					signOut();
					break;
				default:
					deleteAccountAndSignOut();
					break;
				}
			}
		}
		Run();
	}
	void Service::hashString(std::string& hash_target)
	{
		LOG_ALBA_JAVA_METHOD("hashString");
		for (char& c : hash_target)
		{
			c = c % 25;
		}
	}
	bool Service::checkRedundancy(const std::string& id)
	{
		LOG_ALBA_JAVA_METHOD("checkRedundancy");
		return QueryDB::GetInstance()->IsIdAlreadyExist(id);
	}
	bool  Service::doubleCheck(const std::string& in_pw, const std::string& check_pw)
	{
		LOG_ALBA_JAVA_METHOD("doubleCheck");
		return in_pw == check_pw;
	}
	void  Service::signUp()
	{
		LOG_ALBA_JAVA_METHOD("signUp");
		std::string ID;
		std::string PW1;
		std::string PW2;
		std::string input;
		do
		{
			mCLI.Print("ID: ");
			mCLI.GetStringInput(ID);
			if (checkRedundancy(ID))
			{
				mCLI.Print("Same ID is Already exist! Try Again!\n");
			}
			else
				break;
		} while (true);

		do
		{
			mCLI.Print("Password: ");
			mCLI.GetPasswordSafely(PW1);
			mCLI.Print("Confirm password again!: ");
			mCLI.GetPasswordSafely(PW2);
			if (!doubleCheck(PW1, PW2))
			{
				mCLI.Print("Try Again!\n");
			}
			else
				break;
		} while (true);

		mCLI.Print("Choose user type.\n");
		mCLI.Print("1. Individual\n");
		mCLI.Print("Other. Company\n");
		mCLI.Print("input: ");
		mCLI.GetStringInput(input);

		if (input == "1")
		{
			IndividualModel indi;
			mCLI.Print("Name: ");
			mCLI.GetStringInput(indi.Name);
			mCLI.Print("Phone number: ");
			mCLI.GetStringInput(indi.PhoneNumber);
			mCLI.Print("Birthday: ");
			mCLI.GetStringInput(indi.Birthday);
			mCLI.Print("Email: ");
			mCLI.GetStringInput(indi.Email);
			mCLI.Print("Postal code: ");
			mCLI.GetStringInput(indi.PostalCode);
			mCLI.Print("Residence address: ");
			mCLI.GetStringInput(indi.ResidenceAddress);

			hashString(PW1);
			indi.PwHash = PW1;
			if (!QueryDB::GetInstance()->CreateAccountOrFalse(ID, indi))
			{
				std::cout << "System Crashed!" << std::endl;
				exit(1);
			}

		}
		else
		{
			CompanyModel comp;

			mCLI.Print("Name: ");
			mCLI.GetStringInput(comp.Name);
			mCLI.Print("Phone number: ");
			mCLI.GetStringInput(comp.PhoneNumber);
			mCLI.Print("Email: ");
			mCLI.GetStringInput(comp.Email);
			mCLI.Print("Buisness ID: ");
			mCLI.GetStringInput(comp.BuisnessId);


			hashString(PW1);
			comp.PwHash = PW1;
			if (!QueryDB::GetInstance()->CreateAccountOrFalse(ID, comp))
			{
				std::cout << "System Crashed!" << std::endl;
				exit(1);
			}
		}
		std::cout << "Successfully signed up! Return to start page!" << std::endl;


	}

	void Service::updateCompany()
	{
		LOG_ALBA_JAVA_METHOD("updateCompany");
		std::string PW;
		do
		{
			mCLI.Print("Password: ");
			mCLI.GetPasswordSafely(PW);
			hashString(PW);
			if (!doubleCheck(PW, mSessionUserPwHash))
			{
				mCLI.Print("Try Again!\n");
			}
			else
				break;
		} while (true);


		CompanyModel* comp = QueryDB::GetInstance()->ReadCompanyOrNull(mSessionUserId);
		if (comp == nullptr)
		{
			std::cout << "System Crash!" << std::endl;
			exit(1);
		}

		unsigned int input;
		mCLI.Print("Select valid number which you want to modify: \n");
		mCLI.Print("1. Name: ");
		mCLI.Print(comp->Name);
		mCLI.Print("\n2. Phone number: ");
		mCLI.Print(comp->PhoneNumber);
		mCLI.Print("\n3. Email: ");
		mCLI.Print(comp->Email);
		mCLI.Print("\n4. Buisness ID: ");
		mCLI.Print(comp->BuisnessId);
		mCLI.GetNumberInputSafely(input, 1, 4);
		mCLI.ClearScreen();

		switch (input)
		{
		case 1:
			mCLI.Print("Name: ");
			mCLI.GetStringInput(comp->Name);
			break;
		case 2:
			mCLI.Print("Phone number: ");
			mCLI.GetStringInput(comp->PhoneNumber);
			break;
		case 3:
			mCLI.Print("Email: ");
			mCLI.GetStringInput(comp->Email);
			break;
		case 4:
			mCLI.Print("Buisness ID: ");
			mCLI.GetStringInput(comp->BuisnessId);
			break;
		}

		if (!QueryDB::GetInstance()->UpdateAccountOrFalse(mSessionUserId, *comp))
		{
			std::cout << "System Crashed!" << std::endl;
			exit(1);
		}
		delete comp;
	}

	void Service::updateIndividual()
	{
		LOG_ALBA_JAVA_METHOD("updateIndividual");
		std::string PW;
		do
		{
			mCLI.Print("Password: ");
			mCLI.GetPasswordSafely(PW);
			hashString(PW);
			if (!doubleCheck(PW, mSessionUserPwHash))
			{
				mCLI.Print("Try Again!\n");
			}
			else
				break;
		} while (true);


		IndividualModel* indi = QueryDB::GetInstance()->ReadIndividualOrNull(mSessionUserId);
		if (indi == nullptr)
		{
			std::cout << "System Crash!" << std::endl;
			exit(1);
		}

		unsigned int input;
		mCLI.Print("Select valid number which you want to modify: \n");
		mCLI.Print("1. Name: ");
		mCLI.Print(indi->Name);
		mCLI.Print("\n2. Phone number: ");
		mCLI.Print(indi->PhoneNumber);
		mCLI.Print("\n3. Email: ");
		mCLI.Print(indi->Email);
		mCLI.Print("\n4. Birthday: ");
		mCLI.Print(indi->Birthday);
		mCLI.Print("\n5.Postal code: ");
		mCLI.Print(indi->PostalCode);
		mCLI.Print("\n6.Residence address: ");
		mCLI.Print(indi->ResidenceAddress);
		mCLI.GetNumberInputSafely(input, 1, 6);
		mCLI.ClearScreen();

		switch (input)
		{
		case 1:
			mCLI.Print("Name: ");
			mCLI.GetStringInput(indi->Name);
			break;
		case 2:
			mCLI.Print("Phone number: ");
			mCLI.GetStringInput(indi->PhoneNumber);
			break;
		case 3:
			mCLI.Print("Email: ");
			mCLI.GetStringInput(indi->Email);
			break;
		case 4:
			mCLI.Print("Birthday: ");
			mCLI.GetStringInput(indi->Birthday);
			break;
		case 5:
			mCLI.Print("Postal code: ");
			mCLI.GetStringInput(indi->PostalCode);
			break;
		case 6:
			mCLI.Print("Residence address: ");
			mCLI.GetStringInput(indi->ResidenceAddress);
			break;
		}

		if (!QueryDB::GetInstance()->UpdateAccountOrFalse(mSessionUserId, *indi))
		{
			std::cout << "System Crashed!" << std::endl;
			exit(1);
		}
		delete indi;

	}

	void Service::signIn()
	{
		LOG_ALBA_JAVA_METHOD("signIn");
		std::string ID;
		std::string PW;
		unsigned int input;

		while (true)
		{
			mCLI.Print("ID: ");
			mCLI.GetStringInput(ID);
			mCLI.Print("PW: ");
			mCLI.GetPasswordSafely(PW);

			hashString(PW);


			if (QueryDB::GetInstance()->IsCompany(ID, PW) == true)
			{
				mSessionUserId = ID;
				mSessionUserPwHash = PW;
				mIsSessionUserCompany = true;
			}


			if (QueryDB::GetInstance()->IsIndividual(ID, PW) == true)
			{
				mSessionUserId = ID;
				mSessionUserPwHash = PW;
				mIsSessionUserCompany = false;
			}

			if (mSessionUserId.empty())
			{
				mCLI.DisplayBackPage();
				mCLI.GetNumberInputSafely(input, 1, 2);

				if (input != 1)
					return;
			}
			else
			{

				return;
			}

		}



	}
	void Service::signOut()
	{
		LOG_ALBA_JAVA_METHOD("signOut");
		mSessionUserId = "";
		mSessionUserPwHash = "";
	}

	void Service::deleteAccountAndSignOut()
	{
		LOG_ALBA_JAVA_METHOD("deleteAccountAndSignOut");
		std::string PW;
		do
		{
			mCLI.Print("Password: ");
			mCLI.GetPasswordSafely(PW);
			hashString(PW);
			if (!doubleCheck(PW, mSessionUserPwHash))
			{
				mCLI.Print("Try Again!\n");
			}
			else
				break;
		} while (true);
		QueryDB::GetInstance()->DeleteAccount(mSessionUserId);
		mSessionUserId = "";
		mSessionUserPwHash = "";
	}

	void Service::registerCV()
	{
		LOG_ALBA_JAVA_METHOD("registerCV");
		CVModel cv;
		mCLI.Print("Worker name: ");
		mCLI.GetStringInput(cv.WorkerName);
		mCLI.Print("Worker age: ");
		mCLI.GetNumberInputSafely(cv.WorkerAge);
		mCLI.Print("Worker gender: ");
		mCLI.GetStringInput(cv.WorkerGender);
		mCLI.Print("Work place: ");
		mCLI.GetStringInput(cv.WorkPlace);
		mCLI.Print("Worker education: ");
		mCLI.GetStringInput(cv.WorkerEducation);
		mCLI.Print("Worker introduction: ");
		mCLI.GetStringInput(cv.WorkerIntroduction);
		mCLI.Print("Finally, make CV Name: ");
		mCLI.GetStringInput(cv.CVName);
		QueryDB::GetInstance()->CreateCV(cv);


		IndividualModel* indi = QueryDB::GetInstance()->ReadIndividualOrNull(mSessionUserId);
		if (indi == nullptr)
		{
			std::cout << "System Crash!" << std::endl;
			exit(1);
		}

		indi->MyCVIds.push_back(QueryDB::GetInstance()->GetCVCount() - 1);
		QueryDB::GetInstance()->UpdateAccountOrFalse(mSessionUserId, *indi);
		delete indi;
	}


	void Service::updateCV()
	{
		LOG_ALBA_JAVA_METHOD("updateCV");
		unsigned int cv_id;

		IndividualModel* comp = QueryDB::GetInstance()->ReadIndividualOrNull(mSessionUserId);
		if (comp == nullptr)
		{
			std::cout << "System Crash!" << std::endl;
			exit(1);
		}


		std::string input;

		bool should_try_again = true;
		do
		{
			mCLI.Print("Wanna return to menu? input y. : ");
			mCLI.GetStringInput(input);
			if (input == "y")
				return;
			mCLI.Print("\n");
			mCLI.Print("List of your CV_ID is here. \n");
			mCLI.DisplayIDList(comp->MyCVIds);
			mCLI.Print("Select valid CV_ID which you want to modify: ");
			mCLI.GetNumberInputSafely(cv_id);
			for (unsigned int i : comp->MyCVIds)
			{
				if (cv_id == i)
				{
					should_try_again = false;
					break;
				}
			}
		} while (should_try_again);



		CVModel* target = QueryDB::GetInstance()->ReadCVOrNull(cv_id);

		mCLI.Print("Select valid number which you want to modify: \n");
		mCLI.Print("1. Worker name: " + target->WorkerName + "\n");
		mCLI.Print("2. Worker age: " + std::to_string(target->WorkerAge) + "\n");
		mCLI.Print("3. Worker gender: " + target->WorkerGender + "\n");
		mCLI.Print("4. Work place: " + target->WorkPlace + "\n");
		mCLI.Print("5. Worker education: " + target->WorkerEducation + "\n");
		mCLI.Print("6. Worker introduction: " + target->WorkerIntroduction + "\n");
		mCLI.Print("Other input. Nothing to change.\n");
		mCLI.GetStringInput(input);
		if (input == "1")
		{
			mCLI.Print("Worker name: ");
			mCLI.GetStringInput(target->WorkerName);
			mCLI.Print("\n");
		}
		if (input == "2")
		{
			mCLI.Print("Worker age: ");
			mCLI.GetNumberInputSafely(target->WorkerAge);
			mCLI.Print("\n");
		}
		if (input == "3")
		{
			mCLI.Print("Worker gender: ");
			mCLI.GetStringInput(target->WorkerGender);
			mCLI.Print("\n");
		}
		if (input == "4")
		{
			mCLI.Print("Work place: ");
			mCLI.GetStringInput(target->WorkPlace);
			mCLI.Print("\n");
		}
		if (input == "5")
		{
			mCLI.Print("Worker education: ");
			mCLI.GetStringInput(target->WorkerEducation);
			mCLI.Print("\n");
		}
		if (input == "6")
		{
			mCLI.Print("Worker introduction: ");
			mCLI.GetStringInput(target->WorkerIntroduction);
			mCLI.Print("\n");
		}

		if (!QueryDB::GetInstance()->UpdateCVOrFalse(cv_id, *target))
		{
			std::cout << "System Crash!" << std::endl;
			delete target;
			exit(1);
		}
		delete target;
	}

	void Service::deleteCV()
	{
		LOG_ALBA_JAVA_METHOD("deleteCV");
		unsigned int cv_id;
		IndividualModel* indi = QueryDB::GetInstance()->ReadIndividualOrNull(mSessionUserId);
		if (indi == nullptr)
		{
			std::cout << "System Crash!" << std::endl;
			exit(1);
		}
		std::string input;

		bool should_try_again = true;
		do
		{
			mCLI.Print("Wanna return to menu? input y. : ");
			mCLI.GetStringInput(input);
			if (input == "y")
				return;
			mCLI.Print("\n");
			mCLI.Print("List of your CV_ID is here.\n ");
			mCLI.DisplayIDList(indi->MyCVIds);
			mCLI.Print("\n");

			mCLI.Print("Select valid CV_ID which you want to delete: ");
			mCLI.GetNumberInputSafely(cv_id);
			for (unsigned int i : indi->MyCVIds)
			{
				if (cv_id == i)
				{
					should_try_again = false;
					break;
				}
			}
		} while (should_try_again);

		if (!QueryDB::GetInstance()->DeleteCVOrFalse(cv_id))
		{
			std::cout << "System Crash!" << std::endl;
			delete indi;
			exit(1);

		}
		delete indi;
	}

	void Service::searchCV()
	{
		LOG_ALBA_JAVA_METHOD("searchCV");
		unsigned int option;
		mCLI.DisplayCVSearchOption();
		mCLI.GetNumberInputSafely(option, 0, 6);
		std::map<unsigned int, CVModel> CVs;
		unsigned int cv_id_detail = 0;
		std::string input;
		if (option < 6)
		{

			mCLI.Print("Search Keyword: ");
			mCLI.GetStringInput(input);
			CVs = QueryDB::GetInstance()->SearchCV(input, option);
			mCLI.DisplayCVSearchResult(CVs);
		}
		else {

			unsigned int start, end;
			mCLI.Print("Need range! Please input range. (From ~ To ~).\n");
			mCLI.Print("From: ");
			mCLI.GetNumberInputSafely(start);
			mCLI.Print("To: ");
			mCLI.GetNumberInputSafely(end);

			CVs = QueryDB::GetInstance()->SearchCVInRange(option, start, end);
			mCLI.DisplayCVSearchResult(CVs);
		}

		if (CVs.empty())
		{
			mCLI.Print("There is no search results!\n");
			return;
		}

		do
		{

			mCLI.Print("Select valid CV_ID for watch detail. : ");

			mCLI.GetNumberInputSafely(cv_id_detail);

			mCLI.Print("\n");

		} while (CVs.find(cv_id_detail) == CVs.end());
		mCLI.ClearScreen();
		mCLI.DisplayCV(CVs.find(cv_id_detail)->second);
		mCLI.Print("To exit, enter any string. : ");
		mCLI.GetStringInput(input);

	}


	void Service::showAppliedCVs()
	{
		LOG_ALBA_JAVA_METHOD("showAppliedCVs");
		std::map<unsigned int, CVModel> CVs;

		CompanyModel* comp = QueryDB::GetInstance()->ReadCompanyOrNull(mSessionUserId);
		if (comp == nullptr)
		{
			std::cout << "System Crash!" << std::endl;
			exit(1);
		}
		for (const int i : comp->AppliedCVIds)
		{
			const auto* cv = QueryDB::GetInstance()->ReadCVOrNull(i);
			if (cv != nullptr)
				CVs[i] = *cv;
			else
			{
				std::cout << "System Crash!" << std::endl;
				delete comp;
				exit(1);
			}
			delete cv;
			cv = nullptr;
		}
		delete comp;
		if (CVs.empty())
		{
			mCLI.Print("There is no results.\n");
			return;
		}
		mCLI.DisplayCVSearchResult(CVs);
		unsigned int cv_id;
		do
		{

			mCLI.Print("Select valid CV_ID for watch detail. : ");
			mCLI.GetNumberInputSafely(cv_id);
			mCLI.Print("\n");

		} while (CVs.find(cv_id) == CVs.end());
		mCLI.ClearScreen();
		mCLI.DisplayCV(CVs.find(cv_id)->second);
		std::string input;
		mCLI.Print("To exit, enter any string. : ");
		mCLI.GetStringInput(input);
	}

	void Service::applyCV()
	{
		LOG_ALBA_JAVA_METHOD("applyCV");
		unsigned int cv_id, jp_id;
		mCLI.Print("Need CV_ID and JP_ID!.\n");
		mCLI.Print("JP_ID: ");
		mCLI.GetNumberInputSafely(jp_id);
		JPModel* jp = (QueryDB::GetInstance()->ReadJPOrNull(jp_id));
		if (jp == nullptr)
		{
			mCLI.Print("Unknown JP.\n");
			return;
		}

		mCLI.Print("CV_ID: ");
		mCLI.GetNumberInputSafely(cv_id);
		CVModel* cv = (QueryDB::GetInstance()->ReadCVOrNull(cv_id));

		if (cv == nullptr)
		{
			mCLI.Print("Unknown CV.\n");
			return;
		}


		CompanyModel* comp = QueryDB::GetInstance()->ReadCompanyOrNull(jp->CompanyID);
		if (comp == nullptr)
		{
			std::cout << "System Crash!" << std::endl;
			exit(1);
		}

		comp->AppliedCVIds.push_back(cv_id);
		QueryDB::GetInstance()->UpdateAccountOrFalse(jp->CompanyID, *comp);
		delete comp;
		delete jp;
		delete cv;
	}
	void Service::registerJP()
	{
		LOG_ALBA_JAVA_METHOD("registerJP");
		CompanyModel* comp = QueryDB::GetInstance()->ReadCompanyOrNull(mSessionUserId);
		if (comp == nullptr)
		{
			std::cout << "System Crash!" << std::endl;
			exit(1);
		}

		JPModel jp;

		mCLI.Print("Rectal Name: ");
		mCLI.GetStringInput(jp.RectalName);
		mCLI.Print("Occupation: ");
		mCLI.GetStringInput(jp.Occupation);
		mCLI.Print("Region: ");
		mCLI.GetStringInput(jp.Region);
		mCLI.Print("Employment type: ");
		mCLI.GetStringInput(jp.EmployType);
		mCLI.Print("Addition: ");
		mCLI.GetStringInput(jp.Addition);
		mCLI.Print("Hourly wage: ");
		mCLI.GetNumberInputSafely(jp.HourlyWage);
		mCLI.Print("Working period: ");
		mCLI.GetNumberInputSafely(jp.WorkingPeriod);
		mCLI.Print("Working day: ");
		mCLI.GetNumberInputSafely(jp.WorkingDay);
		mCLI.Print("Number Of work day: ");
		mCLI.GetNumberInputSafely(jp.NumOfWorkDay);
		mCLI.Print("Work hours: ");
		mCLI.GetNumberInputSafely(jp.WorkHours);
		mCLI.Print("Holiday pay: ");
		mCLI.GetNumberInputSafely(jp.HolidayPay);
		mCLI.Print("Finally, Give JP a title: ");
		mCLI.GetStringInput(jp.Title);
		jp.CompanyID = mSessionUserId;

		QueryDB::GetInstance()->CreateJP(jp);

		comp->MyJPIds.push_back(QueryDB::GetInstance()->GetJPCount() - 1);
		QueryDB::GetInstance()->UpdateAccountOrFalse(mSessionUserId, *comp);
		delete comp;

	}
	void Service::updateJP()
	{
		LOG_ALBA_JAVA_METHOD("updateJP");
		unsigned int jp_id;

		CompanyModel* comp = QueryDB::GetInstance()->ReadCompanyOrNull(mSessionUserId);
		if (comp == nullptr)
		{
			std::cout << "System Crash!" << std::endl;
			exit(1);
		}


		std::string input;

		bool should_try_again = true;
		do
		{
			mCLI.Print("Wanna return to menu? input y. : ");
			mCLI.GetStringInput(input);
			if (input == "y")
				return;
			mCLI.Print("\n");
			mCLI.Print("List of your JP_ID is here. \n");
			mCLI.DisplayIDList(comp->MyJPIds);
			mCLI.Print("Select valid JP_ID which you want to modify: ");
			mCLI.GetNumberInputSafely(jp_id);
			for (unsigned int i : comp->MyJPIds)
			{
				if (jp_id == i)
				{
					should_try_again = false;
					break;
				}
			}
		} while (should_try_again);


		JPModel* target = QueryDB::GetInstance()->ReadJPOrNull(jp_id);

		mCLI.Print("Select valid number which you want to modify: \n");
		mCLI.Print("0. Title: " + target->Title + "\n");
		mCLI.Print("1. Rectal Name: " + target->RectalName + "\n");
		mCLI.Print("2. Occupation: " + target->Occupation + "\n");
		mCLI.Print("3. Region: " + target->Region + "\n");
		mCLI.Print("4. Employment type: " + target->EmployType + "\n");
		mCLI.Print("5. Additional information: " + target->Addition + "\n");
		mCLI.Print("6. Hourly wage: " + std::to_string(target->HourlyWage) + "\n");
		mCLI.Print("7. Working period: " + std::to_string(target->WorkingPeriod) + "\n");
		mCLI.Print("8. Working day: " + std::to_string(target->WorkingDay) + "\n");
		mCLI.Print("9. Number of working day: " + std::to_string(target->NumOfWorkDay) + "\n");
		mCLI.Print("10. Work hours: " + std::to_string(target->WorkHours) + "\n");
		mCLI.Print("11. Holiday pay: " + std::to_string(target->HolidayPay) + "\n");
		mCLI.Print("Other input. Nothing to change.\n");
		mCLI.GetStringInput(input);
		if (input == "0")
		{
			mCLI.Print("Title: ");
			mCLI.GetStringInput(target->Title);
			mCLI.Print("\n");
		}
		if (input == "1")
		{
			mCLI.Print("Rectal Name: ");
			mCLI.GetStringInput(target->RectalName);
			mCLI.Print("\n");
		}
		if (input == "2")
		{
			mCLI.Print("Occupation: ");
			mCLI.GetStringInput(target->Occupation);
			mCLI.Print("\n");
		}
		if (input == "3")
		{
			mCLI.Print("Region: ");
			mCLI.GetStringInput(target->Region);
			mCLI.Print("\n");
		}
		if (input == "4")
		{
			mCLI.Print("Employment type: ");
			mCLI.GetStringInput(target->EmployType);
			mCLI.Print("\n");
		}
		if (input == "5")
		{
			mCLI.Print("Additional information: ");
			mCLI.GetStringInput(target->Addition);
			mCLI.Print("\n");
		}
		if (input == "6")
		{
			mCLI.Print("Hourly wage: ");
			mCLI.GetNumberInputSafely(target->HourlyWage);
			mCLI.Print("\n");
		}
		if (input == "7")
		{
			mCLI.Print("Working period: ");
			mCLI.GetNumberInputSafely(target->WorkingPeriod);
			mCLI.Print("\n");
		}
		if (input == "8")
		{
			mCLI.Print("Working day: ");
			mCLI.GetNumberInputSafely(target->WorkingDay);
			mCLI.Print("\n");
		}

		if (input == "9")
		{
			mCLI.Print("Number of working day: ");
			mCLI.GetNumberInputSafely(target->NumOfWorkDay);
			mCLI.Print("\n");
		}
		if (input == "10")
		{
			mCLI.Print("Work hours: ");
			mCLI.GetNumberInputSafely(target->WorkHours);
			mCLI.Print("\n");
		}
		if (input == "11")
		{
			mCLI.Print("Holiday pay: ");
			mCLI.GetNumberInputSafely(target->HolidayPay);
			mCLI.Print("\n");
		}
		if (!QueryDB::GetInstance()->UpdateJPOrFalse(jp_id, *target))
		{
			std::cout << "System Crash!" << std::endl;
			delete comp;
			delete target;
			exit(1);
		}
		delete comp;
		delete target;
	}

	void Service::deleteJP()
	{
		unsigned int jp_id;

		LOG_ALBA_JAVA_METHOD("deleteJP");
		CompanyModel* comp = QueryDB::GetInstance()->ReadCompanyOrNull(mSessionUserId);
		if (comp == nullptr)
		{
			std::cout << "System Crash!" << std::endl;
			exit(1);
		}
		std::string input;

		bool should_try_again = true;
		do
		{
			mCLI.Print("Wanna return to menu? input y. : ");
			mCLI.GetStringInput(input);
			if (input == "y")
				return;
			mCLI.Print("\n");
			mCLI.Print("List of your JP_ID is here.\n ");
			mCLI.DisplayIDList(comp->MyJPIds);
			mCLI.Print("\n");

			mCLI.Print("Select valid JP_ID which you want to delete: ");
			mCLI.GetNumberInputSafely(jp_id);
			for (unsigned int i : comp->MyJPIds)
			{
				if (jp_id == i)
				{
					should_try_again = false;
					break;
				}
			}
		} while (should_try_again);

		if (!QueryDB::GetInstance()->DeleteJPOrFalse(jp_id))
		{
			std::cout << "System Crash!" << std::endl;
			delete comp;
			exit(1);
		}
		delete comp;
	}
	void Service::searchJP()
	{
		LOG_ALBA_JAVA_METHOD("searchJP");
		unsigned int option;
		mCLI.DisplayJPSearchOption();
		mCLI.GetNumberInputSafely(option, 0, 11);
		std::map<unsigned int, JPModel> JPs;
		unsigned int jp_id_detail = 0;
		std::string input;
		if (option < 6)
		{

			mCLI.Print("Search Keyword: ");
			mCLI.GetStringInput(input);
			JPs = QueryDB::GetInstance()->SearchJP(input, option);
			mCLI.DisplayJPSearchResult(JPs);
		}
		else {

			unsigned int start, end;
			mCLI.Print("Need range! Please input range. (From ~ To ~).\n");
			mCLI.Print("From: ");
			mCLI.GetNumberInputSafely(start);
			mCLI.Print("To: ");
			mCLI.GetNumberInputSafely(end);

			JPs = QueryDB::GetInstance()->SearchJPInRange(option, start, end);
			mCLI.DisplayJPSearchResult(JPs);
		}

		if (JPs.empty())
		{
			mCLI.Print("There is no search results!\n");
			return;
		}

		do
		{
			mCLI.Print("Select valid JP_ID for watch detail. : ");
			mCLI.GetNumberInputSafely(jp_id_detail);
			mCLI.Print("\n");

		} while (JPs.find(jp_id_detail) == JPs.end());
		mCLI.ClearScreen();
		mCLI.DisplayJP(JPs.find(jp_id_detail)->second);
		mCLI.Print("To exit, enter any string. : ");
		mCLI.GetStringInput(input);
	}

}