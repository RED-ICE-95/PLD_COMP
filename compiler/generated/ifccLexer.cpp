
// Generated from ifcc.g4 by ANTLR 4.13.2


#include "ifccLexer.h"


using namespace antlr4;



using namespace antlr4;

namespace {

struct IfccLexerStaticData final {
  IfccLexerStaticData(std::vector<std::string> ruleNames,
                          std::vector<std::string> channelNames,
                          std::vector<std::string> modeNames,
                          std::vector<std::string> literalNames,
                          std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), channelNames(std::move(channelNames)),
        modeNames(std::move(modeNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  IfccLexerStaticData(const IfccLexerStaticData&) = delete;
  IfccLexerStaticData(IfccLexerStaticData&&) = delete;
  IfccLexerStaticData& operator=(const IfccLexerStaticData&) = delete;
  IfccLexerStaticData& operator=(IfccLexerStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> channelNames;
  const std::vector<std::string> modeNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag ifcclexerLexerOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
std::unique_ptr<IfccLexerStaticData> ifcclexerLexerStaticData = nullptr;

void ifcclexerLexerInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (ifcclexerLexerStaticData != nullptr) {
    return;
  }
#else
  assert(ifcclexerLexerStaticData == nullptr);
#endif
  auto staticData = std::make_unique<IfccLexerStaticData>(
    std::vector<std::string>{
      "T__0", "T__1", "T__2", "T__3", "T__4", "T__5", "T__6", "T__7", "T__8", 
      "T__9", "T__10", "T__11", "T__12", "T__13", "T__14", "T__15", "T__16", 
      "T__17", "T__18", "T__19", "T__20", "T__21", "RETURN", "CONST", "ID", 
      "COMMENT", "DIRECTIVE", "WS"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
    },
    std::vector<std::string>{
      "", "'int'", "'main'", "'('", "')'", "'{'", "'}'", "';'", "'='", "','", 
      "'|'", "'^'", "'&'", "'=='", "'!='", "'<'", "'>'", "'+'", "'-'", "'*'", 
      "'/'", "'%'", "'!'", "'return'"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "RETURN", "CONST", "ID", "COMMENT", "DIRECTIVE", 
      "WS"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,0,28,156,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,
  	6,2,7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,
  	7,14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,
  	7,21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,1,0,
  	1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,2,1,2,1,3,1,3,1,4,1,4,1,5,1,5,1,6,1,
  	6,1,7,1,7,1,8,1,8,1,9,1,9,1,10,1,10,1,11,1,11,1,12,1,12,1,12,1,13,1,13,
  	1,13,1,14,1,14,1,15,1,15,1,16,1,16,1,17,1,17,1,18,1,18,1,19,1,19,1,20,
  	1,20,1,21,1,21,1,22,1,22,1,22,1,22,1,22,1,22,1,22,1,23,4,23,117,8,23,
  	11,23,12,23,118,1,24,1,24,5,24,123,8,24,10,24,12,24,126,9,24,1,25,1,25,
  	1,25,1,25,5,25,132,8,25,10,25,12,25,135,9,25,1,25,1,25,1,25,1,25,1,25,
  	1,26,1,26,5,26,144,8,26,10,26,12,26,147,9,26,1,26,1,26,1,26,1,26,1,27,
  	1,27,1,27,1,27,2,133,145,0,28,1,1,3,2,5,3,7,4,9,5,11,6,13,7,15,8,17,9,
  	19,10,21,11,23,12,25,13,27,14,29,15,31,16,33,17,35,18,37,19,39,20,41,
  	21,43,22,45,23,47,24,49,25,51,26,53,27,55,28,1,0,4,1,0,48,57,3,0,65,90,
  	95,95,97,122,4,0,48,57,65,90,95,95,97,122,3,0,9,10,13,13,32,32,159,0,
  	1,1,0,0,0,0,3,1,0,0,0,0,5,1,0,0,0,0,7,1,0,0,0,0,9,1,0,0,0,0,11,1,0,0,
  	0,0,13,1,0,0,0,0,15,1,0,0,0,0,17,1,0,0,0,0,19,1,0,0,0,0,21,1,0,0,0,0,
  	23,1,0,0,0,0,25,1,0,0,0,0,27,1,0,0,0,0,29,1,0,0,0,0,31,1,0,0,0,0,33,1,
  	0,0,0,0,35,1,0,0,0,0,37,1,0,0,0,0,39,1,0,0,0,0,41,1,0,0,0,0,43,1,0,0,
  	0,0,45,1,0,0,0,0,47,1,0,0,0,0,49,1,0,0,0,0,51,1,0,0,0,0,53,1,0,0,0,0,
  	55,1,0,0,0,1,57,1,0,0,0,3,61,1,0,0,0,5,66,1,0,0,0,7,68,1,0,0,0,9,70,1,
  	0,0,0,11,72,1,0,0,0,13,74,1,0,0,0,15,76,1,0,0,0,17,78,1,0,0,0,19,80,1,
  	0,0,0,21,82,1,0,0,0,23,84,1,0,0,0,25,86,1,0,0,0,27,89,1,0,0,0,29,92,1,
  	0,0,0,31,94,1,0,0,0,33,96,1,0,0,0,35,98,1,0,0,0,37,100,1,0,0,0,39,102,
  	1,0,0,0,41,104,1,0,0,0,43,106,1,0,0,0,45,108,1,0,0,0,47,116,1,0,0,0,49,
  	120,1,0,0,0,51,127,1,0,0,0,53,141,1,0,0,0,55,152,1,0,0,0,57,58,5,105,
  	0,0,58,59,5,110,0,0,59,60,5,116,0,0,60,2,1,0,0,0,61,62,5,109,0,0,62,63,
  	5,97,0,0,63,64,5,105,0,0,64,65,5,110,0,0,65,4,1,0,0,0,66,67,5,40,0,0,
  	67,6,1,0,0,0,68,69,5,41,0,0,69,8,1,0,0,0,70,71,5,123,0,0,71,10,1,0,0,
  	0,72,73,5,125,0,0,73,12,1,0,0,0,74,75,5,59,0,0,75,14,1,0,0,0,76,77,5,
  	61,0,0,77,16,1,0,0,0,78,79,5,44,0,0,79,18,1,0,0,0,80,81,5,124,0,0,81,
  	20,1,0,0,0,82,83,5,94,0,0,83,22,1,0,0,0,84,85,5,38,0,0,85,24,1,0,0,0,
  	86,87,5,61,0,0,87,88,5,61,0,0,88,26,1,0,0,0,89,90,5,33,0,0,90,91,5,61,
  	0,0,91,28,1,0,0,0,92,93,5,60,0,0,93,30,1,0,0,0,94,95,5,62,0,0,95,32,1,
  	0,0,0,96,97,5,43,0,0,97,34,1,0,0,0,98,99,5,45,0,0,99,36,1,0,0,0,100,101,
  	5,42,0,0,101,38,1,0,0,0,102,103,5,47,0,0,103,40,1,0,0,0,104,105,5,37,
  	0,0,105,42,1,0,0,0,106,107,5,33,0,0,107,44,1,0,0,0,108,109,5,114,0,0,
  	109,110,5,101,0,0,110,111,5,116,0,0,111,112,5,117,0,0,112,113,5,114,0,
  	0,113,114,5,110,0,0,114,46,1,0,0,0,115,117,7,0,0,0,116,115,1,0,0,0,117,
  	118,1,0,0,0,118,116,1,0,0,0,118,119,1,0,0,0,119,48,1,0,0,0,120,124,7,
  	1,0,0,121,123,7,2,0,0,122,121,1,0,0,0,123,126,1,0,0,0,124,122,1,0,0,0,
  	124,125,1,0,0,0,125,50,1,0,0,0,126,124,1,0,0,0,127,128,5,47,0,0,128,129,
  	5,42,0,0,129,133,1,0,0,0,130,132,9,0,0,0,131,130,1,0,0,0,132,135,1,0,
  	0,0,133,134,1,0,0,0,133,131,1,0,0,0,134,136,1,0,0,0,135,133,1,0,0,0,136,
  	137,5,42,0,0,137,138,5,47,0,0,138,139,1,0,0,0,139,140,6,25,0,0,140,52,
  	1,0,0,0,141,145,5,35,0,0,142,144,9,0,0,0,143,142,1,0,0,0,144,147,1,0,
  	0,0,145,146,1,0,0,0,145,143,1,0,0,0,146,148,1,0,0,0,147,145,1,0,0,0,148,
  	149,5,10,0,0,149,150,1,0,0,0,150,151,6,26,0,0,151,54,1,0,0,0,152,153,
  	7,3,0,0,153,154,1,0,0,0,154,155,6,27,1,0,155,56,1,0,0,0,5,0,118,124,133,
  	145,2,6,0,0,0,1,0
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  ifcclexerLexerStaticData = std::move(staticData);
}

}

ifccLexer::ifccLexer(CharStream *input) : Lexer(input) {
  ifccLexer::initialize();
  _interpreter = new atn::LexerATNSimulator(this, *ifcclexerLexerStaticData->atn, ifcclexerLexerStaticData->decisionToDFA, ifcclexerLexerStaticData->sharedContextCache);
}

ifccLexer::~ifccLexer() {
  delete _interpreter;
}

std::string ifccLexer::getGrammarFileName() const {
  return "ifcc.g4";
}

const std::vector<std::string>& ifccLexer::getRuleNames() const {
  return ifcclexerLexerStaticData->ruleNames;
}

const std::vector<std::string>& ifccLexer::getChannelNames() const {
  return ifcclexerLexerStaticData->channelNames;
}

const std::vector<std::string>& ifccLexer::getModeNames() const {
  return ifcclexerLexerStaticData->modeNames;
}

const dfa::Vocabulary& ifccLexer::getVocabulary() const {
  return ifcclexerLexerStaticData->vocabulary;
}

antlr4::atn::SerializedATNView ifccLexer::getSerializedATN() const {
  return ifcclexerLexerStaticData->serializedATN;
}

const atn::ATN& ifccLexer::getATN() const {
  return *ifcclexerLexerStaticData->atn;
}




void ifccLexer::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  ifcclexerLexerInitialize();
#else
  ::antlr4::internal::call_once(ifcclexerLexerOnceFlag, ifcclexerLexerInitialize);
#endif
}
