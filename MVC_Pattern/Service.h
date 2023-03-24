#pragma once
#include "View.h"
#include <climits>

namespace alba_java
{
	class Service
	{
	public:
		void Run();
		void Update();
		Service(std::ostream& out_stream, std::istream& in_stream)
			:mCLI(out_stream, in_stream)
		{

		}
	private:
		std::string mSessionUserId = "";
		std::string mSessionUserPwHash = "";
		View mCLI;
		bool mIsSessionUserCompany = false;

		static bool doubleCheck(const std::string& in_pw, const std::string& check_pw);
		bool checkRedundancy(const std::string& id);
		static void hashString(std::string& hash_target);
		void signIn();
		void signUp();
		void updateCompany();
		void updateIndividual();
		void signOut();
		void deleteAccountAndSignOut();

		void registerCV();
		void searchCV();
		void updateCV();
		void deleteCV();
		void applyCV();
		void showAppliedCVs();

		void registerJP();
		void searchJP();
		void updateJP();
		void deleteJP();
	};





}