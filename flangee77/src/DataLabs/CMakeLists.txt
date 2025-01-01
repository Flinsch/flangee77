
target_sources(flangee77
    PRIVATE
    Base64.cpp
    Base64.h
    compression.h

    compression/Deflate.cpp
    compression/Deflate.h

    syntax/Diagnostic.h
    syntax/Diagnostics.cpp
    syntax/Diagnostics.h
    syntax/GenericLexer.cpp
    syntax/GenericLexer.h
    syntax/Grammar.cpp
    syntax/Grammar.h
    syntax/Lexer.cpp
    syntax/Lexer.h
    syntax/Parser.cpp
    syntax/Parser.h
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
)
