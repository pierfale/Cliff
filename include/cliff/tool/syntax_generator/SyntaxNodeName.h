#ifndef _CLIFF_SYNTAX_NODE_NAME_H
#define _CLIFF_SYNTAX_NODE_NAME_H

namespace cliff {

	class SyntaxNodeName {

	public:
		static constexpr const char* RegularExpression = "regular_expression";

		static constexpr const char* RegularExpressionAlternative = "regular_expression_alternative";
		static constexpr const char* RegularExpressionRepetition = "regular_expression_repetition";

		static constexpr const char* RegularExpressionRange = "regular_expression_range";
		static constexpr const char* RegularExpressionLetter = "regular_expression_letter";
	};
}
#endif
