#pragma once
#include "lexer.hpp"
namespace Mer
{
	enum ESymbol
	{
		SFUN,SVAR,SNAME,
	};
	struct WordRecorder
	{
		Token *word;
		ESymbol es;
		int word_type;
	};

}