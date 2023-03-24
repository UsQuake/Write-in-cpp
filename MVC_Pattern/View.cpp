
#include "View.h"
namespace alba_java
{

	void  View::Print(const std::string& out)
	{
		LOG_ALBA_JAVA_METHOD("Print");
		mOutStream << out;
	}
	void  View::PrintNumber(const int out)
	{
		LOG_ALBA_JAVA_METHOD("PrintNumber");
		mOutStream << out;
	}
	void  View::GetStringInput(std::string& in)
	{
		LOG_ALBA_JAVA_METHOD("GetStringInput");
		mInStream >> in;
	}
	void View::GetPasswordSafely(std::string& pw)
	{
		LOG_ALBA_JAVA_METHOD("GetPasswordSafely");

#ifdef _WIN32
		HANDLE hnd_terminal = GetStdHandle(STD_INPUT_HANDLE);
		DWORD mode;
		GetConsoleMode(hnd_terminal, &mode);
		mode &= ~ENABLE_ECHO_INPUT;
		SetConsoleMode(hnd_terminal, mode);

#else
		struct termios ptr_terminal;
		tcgetattr(STDIN_FILENO, &ptr_terminal);
		ptr_terminal.c_lflag &= ~ECHO;
		(void)tcsetattr(STDIN_FILENO, TCSANOW, &ptr_terminal);
#endif
		mInStream >> pw;

#ifdef _WIN32
		hnd_terminal = GetStdHandle(STD_INPUT_HANDLE);
		mode |= ENABLE_ECHO_INPUT;
		SetConsoleMode(hnd_terminal, mode);

#else
		tcgetattr(STDIN_FILENO, &ptr_terminal);
		ptr_terminal.c_lflag |= ECHO;
		(void)tcsetattr(STDIN_FILENO, TCSANOW, &ptr_terminal);
#endif
		mOutStream << std::endl;
	}
	void View::GetNumberInputSafely(unsigned int& num)
	{
		LOG_ALBA_JAVA_METHOD("GetNumberInputSafely");
		while (true)
		{
			mInStream >> num;

			if (!mInStream.fail())
			{
				break;
			}
			else
			{
				mInStream.clear();
				mInStream.ignore(LLONG_MAX, '\n');
				mOutStream << "Please write valid number! Input again : ";
			}
		}
	}
	void View::GetNumberInputSafely(unsigned int& num, unsigned int st, unsigned int end)
	{
		LOG_ALBA_JAVA_METHOD("GetNumberInputSafely");
		while (true)
		{
			mInStream >> num;

			if (!mInStream.fail())
			{
				if (st <= num && num <= end)
					break;
			}
			else
			{
				mInStream.clear();
				mInStream.ignore(LLONG_MAX, '\n');
			}
			mOutStream << "Please write valid number! Input again : ";
		}
	}
	void View::DisplayStartPage()
	{
		LOG_ALBA_JAVA_METHOD("DisplayStartPage");
		mOutStream << "Welcome To Alba-Java Service!" << std::endl;
		mOutStream << "Input number you want!!" << std::endl;
		mOutStream << "1. Sign in." << std::endl;
		mOutStream << "2. Sign up." << std::endl;
		mOutStream << "3. Exit." << std::endl;

	}
	void View::DisplayIndividualMenu()
	{
		LOG_ALBA_JAVA_METHOD("DisplayIndividualMenu");
		mOutStream << "Select option." << std::endl;
		mOutStream << "0. Update account." << std::endl;
		mOutStream << "1. Search JPs" << std::endl;
		mOutStream << "2. Create my CV." << std::endl;
		mOutStream << "3. Modify my CV." << std::endl;
		mOutStream << "4. Delete my CV." << std::endl;
		mOutStream << "5. Apply CV." << std::endl;
		mOutStream << "6. Exit(Sign Out)." << std::endl;
		mOutStream << "7. Exit(Remove Account)." << std::endl;
		mOutStream << "Input : ";

	}

	void View::DisplayCompanyMenu()
	{
		LOG_ALBA_JAVA_METHOD("DisplayCompanyMenu");
		mOutStream << "Select option." << std::endl;
		mOutStream << "0. Update account." << std::endl;
		mOutStream << "1. Search CV." << std::endl;
		mOutStream << "2. Create my job posting." << std::endl;
		mOutStream << "3. Modify my job posting." << std::endl;
		mOutStream << "4. Delete my job posting." << std::endl;
		mOutStream << "5. Show applied CVs." << std::endl;
		mOutStream << "6. Exit(Sign Out)." << std::endl;
		mOutStream << "7. Exit(Remove Account)." << std::endl;
		mOutStream << "Input : ";
	}
	void View::ClearScreen()
	{
		//#ifdef _WIN32
		//		system("cls");
		//#else
		//		system("clear");
		//#endif
	}

	void View::DisplayBackPage()
	{
		LOG_ALBA_JAVA_METHOD("DisplayBackPage");
		mOutStream << "Choose action." << std::endl;
		mOutStream << "1. Try again." << std::endl;
		mOutStream << "2. Back to start menu." << std::endl;
		mOutStream << "input: ";
	}


	void View::DisplayJPSearchResult(const std::map<unsigned int, JPModel>& JPs)
	{
		LOG_ALBA_JAVA_METHOD("DisplayJPSearchResult");
		for (const auto& idx_jp : JPs)
		{
			mOutStream << "JP_ID: " << idx_jp.first << std::endl
				<< "Title: " << idx_jp.second.Title << std::endl
				<< "Rectal name: " << idx_jp.second.RectalName << std::endl << std::endl;
		}
	}

	void View::DisplayJP(const JPModel& JP)
	{
		LOG_ALBA_JAVA_METHOD("DisplayJP");

		mOutStream << "Title: " << JP.Title << std::endl;
		mOutStream << "Rectal name: " << JP.RectalName << std::endl;
		mOutStream << "Occupation: " << JP.Occupation << std::endl;
		mOutStream << "Region: " << JP.Region << std::endl;
		mOutStream << "Employment type: " << JP.EmployType << std::endl;
		mOutStream << "Additional information: " << JP.Addition << std::endl;
		mOutStream << "Hourly wage: " << JP.HourlyWage << std::endl;
		mOutStream << "Working period: " << JP.WorkingPeriod << std::endl;
		mOutStream << "Working day: " << JP.WorkingDay << std::endl;
		mOutStream << "Number of working day: " << JP.NumOfWorkDay << std::endl;
		mOutStream << "Work hours: " << JP.WorkHours << std::endl;
		mOutStream << "Holiday pay: " << JP.HolidayPay << std::endl;
	}



	void View::DisplayJPSearchOption()
	{
		LOG_ALBA_JAVA_METHOD("DisplayJPSearchOption");
		mOutStream << "Search option:\n";
		mOutStream << "0. Rectal name\n";
		mOutStream << "1. Title\n";
		mOutStream << "2. Occupation\n";
		mOutStream << "3. Region\n";
		mOutStream << "4. Employ type\n";
		mOutStream << "5. Additional information\n";
		mOutStream << "6. Hourly wage\n";
		mOutStream << "7. Working period\n";
		mOutStream << "8. Working day\n";
		mOutStream << "9. Number of working day\n";
		mOutStream << "10. Work hours\n";
		mOutStream << "11. Holiday pay\n";
	}

	void View::DisplayIDList(const std::vector<unsigned int>& ids)
	{
		LOG_ALBA_JAVA_METHOD("DisplayIDList");
		for (const auto& idx : ids)
		{
			mOutStream << "ID: " << idx << std::endl;
		}
		mOutStream << std::endl;
	}

	void View::DisplayCVSearchResult(const std::map<unsigned int, CVModel>& CVs)
	{
		LOG_ALBA_JAVA_METHOD("DisplayCVSearchResult");
		for (const auto& idx_cv : CVs)
		{
			mOutStream << "CV_ID: " << idx_cv.first << std::endl
				<< " Title: " << idx_cv.second.CVName << std::endl
				<< " Worker name: " << idx_cv.second.WorkerName << std::endl << std::endl;
		}

	}

	void View::DisplayCVSearchOption()
	{
		LOG_ALBA_JAVA_METHOD("DisplayCVSearchOption");
		mOutStream << "Search option: " << std::endl;
		mOutStream << "0. Title" << std::endl;
		mOutStream << "1. Worker name" << std::endl;
		mOutStream << "2. Worker gender" << std::endl;
		mOutStream << "3. Working place" << std::endl;
		mOutStream << "4. Worker education" << std::endl;
		mOutStream << "5. Worker introduction" << std::endl;
		mOutStream << "6. Worker age" << std::endl;
	}

	void View::DisplayCV(const CVModel& CV)
	{
		LOG_ALBA_JAVA_METHOD("DisplayCV");
		mOutStream << "Title: " << CV.CVName << std::endl;
		mOutStream << "Worker name: " << CV.WorkerName << std::endl;
		mOutStream << "Worker age: " << CV.WorkerAge << std::endl;
		mOutStream << "Worker gender: " << CV.WorkerGender << std::endl;
		mOutStream << "Work place: " << CV.WorkPlace << std::endl;
		mOutStream << "Worker education: " << CV.WorkerEducation << std::endl;
		mOutStream << "Worker introduction: " << CV.WorkerIntroduction << std::endl;
	}
}