
target_sources(flangee77
    PRIVATE
    Base64.cpp
    Base64.h
    compression.h

    compression/Deflate.cpp
    compression/Deflate.h

    json/factory.cpp
    json/factory.h
    json/json.cpp
    json/json.h
    json/object.cpp
    json/object.h
    json/types.h

    syntax/Diagnostic.h
    syntax/Diagnostics.cpp
    syntax/Diagnostics.h
    syntax/GenericLexer.cpp
    syntax/GenericLexer.h
    syntax/Grammar.h
    syntax/GrammarAnalyzer.cpp
    syntax/GrammarAnalyzer.h
    syntax/Lexer.cpp
    syntax/Lexer.h
    syntax/Parser.cpp
    syntax/Parser.h
    syntax/ProductionRule.cpp
    syntax/ProductionRule.h
    syntax/ProductionRuleCollection.cpp
    syntax/ProductionRuleCollection.h
    syntax/SourceLocation.h
    syntax/SymbolID.h
    syntax/TerminalSymbol.cpp
    syntax/TerminalSymbol.h
    syntax/TerminalSymbolCollection.cpp
    syntax/TerminalSymbolCollection.h
    syntax/Token.h
    syntax/Vocabulary.cpp
    syntax/Vocabulary.h
)
