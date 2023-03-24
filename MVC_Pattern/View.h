#pragma once
#include "QueryDB.h"
#include <climits>
#ifdef _WIN32
#include <Windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif


namespace alba_java
{


	class View {
	private:
		std::ostream& mOutStream;
		std::istream& mInStream;


	public:
		View(std::ostream& out_stream, std::istream& in_stream)
			:mOutStream(out_stream)
			, mInStream(in_stream)
		{

		}
		~View()
		{

		}
		void DisplayStartPage();
		void DisplayBackPage();;
		void DisplayIndividualMenu();
		void DisplayCompanyMenu();

		void Print(const std::string& out);
		void PrintNumber(const int out);
		void GetStringInput(std::string& in);
		void GetPasswordSafely(std::string& pw);
		void GetNumberInputSafely(unsigned int& num);
		void GetNumberInputSafely(unsigned int& num, unsigned int st, unsigned int end);
		void ClearScreen();


		void DisplayJPSearchOption();
		void DisplayJPSearchResult(const std::map<unsigned int, JPModel>& JPs);
		void DisplayJP(const JPModel& JP);

		void DisplayCVSearchOption();
		void DisplayCVSearchResult(const std::map<unsigned int, CVModel>& CVs);
		void DisplayCV(const CVModel& CV);
		void DisplayIDList(const std::vector<unsigned int>& ids);
	};

}