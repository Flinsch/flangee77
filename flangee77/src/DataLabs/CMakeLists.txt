
target_sources(flangee77
    PRIVATE
    Base64.cpp
    Base64.h
    compression.h

    compression/Deflate.cpp
    compression/Deflate.h

    json/Format.h
    json/Json.cpp
    json/Json.h
    json/JsonReader.cpp
    json/JsonReader.h
    json/JsonWriter.cpp
    json/JsonWriter.h
    json/types.h

    json/detail/Builder.cpp
    json/detail/Builder.h
    json/detail/Generator.cpp
    json/detail/Generator.h
    json/detail/IdentifierMatcher.cpp
    json/detail/IdentifierMatcher.h
    json/detail/Lexer.cpp
    json/detail/Lexer.h
    json/detail/Symbol.h

    syntax/AstNode.h
    syntax/AstProcessor.h
    syntax/BufferedTokenReader.h
    syntax/CstBuilder.h
    syntax/CstNode.cpp
    syntax/CstNode.h
    syntax/CstToAstTransformer.h
    syntax/Diagnostic.cpp
    syntax/Diagnostic.h
    syntax/Diagnostics.cpp
    syntax/Diagnostics.h
    syntax/DirectAstBuilder.h
    syntax/GenericLexer.cpp
    syntax/GenericLexer.h
    syntax/Grammar.h
    syntax/GrammarAnalyzer.cpp
    syntax/GrammarAnalyzer.h
    syntax/Lexer.cpp
    syntax/Lexer.h
    syntax/LexingTokenReader.h
    syntax/matchers.cpp
    syntax/matchers.h
    syntax/ParseStage.cpp
    syntax/ParseStage.h
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
    syntax/TokenReader.h
    syntax/Vocabulary.cpp
    syntax/Vocabulary.h
)
