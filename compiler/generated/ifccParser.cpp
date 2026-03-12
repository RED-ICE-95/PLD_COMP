
// Generated from ifcc.g4 by ANTLR 4.13.2


#include "ifccVisitor.h"

#include "ifccParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct IfccParserStaticData final {
  IfccParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  IfccParserStaticData(const IfccParserStaticData&) = delete;
  IfccParserStaticData(IfccParserStaticData&&) = delete;
  IfccParserStaticData& operator=(const IfccParserStaticData&) = delete;
  IfccParserStaticData& operator=(IfccParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag ifccParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
std::unique_ptr<IfccParserStaticData> ifccParserStaticData = nullptr;

void ifccParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (ifccParserStaticData != nullptr) {
    return;
  }
#else
  assert(ifccParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<IfccParserStaticData>(
    std::vector<std::string>{
      "axiom", "prog", "stmt", "return_stmt", "assign", "declar", "block", 
      "expr"
    },
    std::vector<std::string>{
      "", "'int'", "'main'", "'('", "')'", "'{'", "'}'", "';'", "'='", "','", 
      "'-'", "'!'", "'*'", "'/'", "'%'", "'+'", "'<'", "'>'", "'=='", "'!='", 
      "'&'", "'^'", "'|'", "'return'"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "RETURN", "CONST", "CHAR_CONST", "ID", "COMMENT", 
      "LINE_COMMENT", "DIRECTIVE", "WS"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,30,108,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,5,1,26,8,1,10,1,12,1,29,9,1,
  	1,1,1,1,1,2,1,2,1,2,3,2,36,8,2,1,2,1,2,1,2,3,2,41,8,2,1,3,1,3,1,3,1,4,
  	1,4,1,4,1,4,1,5,1,5,1,5,1,5,5,5,54,8,5,10,5,12,5,57,9,5,1,6,1,6,5,6,61,
  	8,6,10,6,12,6,64,9,6,1,6,1,6,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,
  	1,7,1,7,3,7,80,8,7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,
  	7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,5,7,103,8,7,10,7,12,7,106,9,7,1,7,0,
  	1,14,8,0,2,4,6,8,10,12,14,0,4,1,0,12,14,2,0,10,10,15,15,1,0,16,17,1,0,
  	18,19,117,0,16,1,0,0,0,2,19,1,0,0,0,4,40,1,0,0,0,6,42,1,0,0,0,8,45,1,
  	0,0,0,10,49,1,0,0,0,12,58,1,0,0,0,14,79,1,0,0,0,16,17,3,2,1,0,17,18,5,
  	0,0,1,18,1,1,0,0,0,19,20,5,1,0,0,20,21,5,2,0,0,21,22,5,3,0,0,22,23,5,
  	4,0,0,23,27,5,5,0,0,24,26,3,4,2,0,25,24,1,0,0,0,26,29,1,0,0,0,27,25,1,
  	0,0,0,27,28,1,0,0,0,28,30,1,0,0,0,29,27,1,0,0,0,30,31,5,6,0,0,31,3,1,
  	0,0,0,32,36,3,6,3,0,33,36,3,8,4,0,34,36,3,10,5,0,35,32,1,0,0,0,35,33,
  	1,0,0,0,35,34,1,0,0,0,36,37,1,0,0,0,37,38,5,7,0,0,38,41,1,0,0,0,39,41,
  	3,12,6,0,40,35,1,0,0,0,40,39,1,0,0,0,41,5,1,0,0,0,42,43,5,23,0,0,43,44,
  	3,14,7,0,44,7,1,0,0,0,45,46,5,26,0,0,46,47,5,8,0,0,47,48,3,14,7,0,48,
  	9,1,0,0,0,49,50,5,1,0,0,50,55,5,26,0,0,51,52,5,9,0,0,52,54,5,26,0,0,53,
  	51,1,0,0,0,54,57,1,0,0,0,55,53,1,0,0,0,55,56,1,0,0,0,56,11,1,0,0,0,57,
  	55,1,0,0,0,58,62,5,5,0,0,59,61,3,4,2,0,60,59,1,0,0,0,61,64,1,0,0,0,62,
  	60,1,0,0,0,62,63,1,0,0,0,63,65,1,0,0,0,64,62,1,0,0,0,65,66,5,6,0,0,66,
  	13,1,0,0,0,67,68,6,7,-1,0,68,69,5,10,0,0,69,80,3,14,7,13,70,71,5,11,0,
  	0,71,80,3,14,7,12,72,73,5,3,0,0,73,74,3,14,7,0,74,75,5,4,0,0,75,80,1,
  	0,0,0,76,80,5,24,0,0,77,80,5,25,0,0,78,80,5,26,0,0,79,67,1,0,0,0,79,70,
  	1,0,0,0,79,72,1,0,0,0,79,76,1,0,0,0,79,77,1,0,0,0,79,78,1,0,0,0,80,104,
  	1,0,0,0,81,82,10,7,0,0,82,83,7,0,0,0,83,103,3,14,7,8,84,85,10,6,0,0,85,
  	86,7,1,0,0,86,103,3,14,7,7,87,88,10,5,0,0,88,89,7,2,0,0,89,103,3,14,7,
  	6,90,91,10,4,0,0,91,92,7,3,0,0,92,103,3,14,7,5,93,94,10,3,0,0,94,95,5,
  	20,0,0,95,103,3,14,7,4,96,97,10,2,0,0,97,98,5,21,0,0,98,103,3,14,7,3,
  	99,100,10,1,0,0,100,101,5,22,0,0,101,103,3,14,7,2,102,81,1,0,0,0,102,
  	84,1,0,0,0,102,87,1,0,0,0,102,90,1,0,0,0,102,93,1,0,0,0,102,96,1,0,0,
  	0,102,99,1,0,0,0,103,106,1,0,0,0,104,102,1,0,0,0,104,105,1,0,0,0,105,
  	15,1,0,0,0,106,104,1,0,0,0,8,27,35,40,55,62,79,102,104
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  ifccParserStaticData = std::move(staticData);
}

}

ifccParser::ifccParser(TokenStream *input) : ifccParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

ifccParser::ifccParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  ifccParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *ifccParserStaticData->atn, ifccParserStaticData->decisionToDFA, ifccParserStaticData->sharedContextCache, options);
}

ifccParser::~ifccParser() {
  delete _interpreter;
}

const atn::ATN& ifccParser::getATN() const {
  return *ifccParserStaticData->atn;
}

std::string ifccParser::getGrammarFileName() const {
  return "ifcc.g4";
}

const std::vector<std::string>& ifccParser::getRuleNames() const {
  return ifccParserStaticData->ruleNames;
}

const dfa::Vocabulary& ifccParser::getVocabulary() const {
  return ifccParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView ifccParser::getSerializedATN() const {
  return ifccParserStaticData->serializedATN;
}


//----------------- AxiomContext ------------------------------------------------------------------

ifccParser::AxiomContext::AxiomContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ifccParser::ProgContext* ifccParser::AxiomContext::prog() {
  return getRuleContext<ifccParser::ProgContext>(0);
}

tree::TerminalNode* ifccParser::AxiomContext::EOF() {
  return getToken(ifccParser::EOF, 0);
}


size_t ifccParser::AxiomContext::getRuleIndex() const {
  return ifccParser::RuleAxiom;
}


std::any ifccParser::AxiomContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitAxiom(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::AxiomContext* ifccParser::axiom() {
  AxiomContext *_localctx = _tracker.createInstance<AxiomContext>(_ctx, getState());
  enterRule(_localctx, 0, ifccParser::RuleAxiom);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(16);
    prog();
    setState(17);
    match(ifccParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ProgContext ------------------------------------------------------------------

ifccParser::ProgContext::ProgContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ifccParser::StmtContext *> ifccParser::ProgContext::stmt() {
  return getRuleContexts<ifccParser::StmtContext>();
}

ifccParser::StmtContext* ifccParser::ProgContext::stmt(size_t i) {
  return getRuleContext<ifccParser::StmtContext>(i);
}


size_t ifccParser::ProgContext::getRuleIndex() const {
  return ifccParser::RuleProg;
}


std::any ifccParser::ProgContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitProg(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::ProgContext* ifccParser::prog() {
  ProgContext *_localctx = _tracker.createInstance<ProgContext>(_ctx, getState());
  enterRule(_localctx, 2, ifccParser::RuleProg);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(19);
    match(ifccParser::T__0);
    setState(20);
    match(ifccParser::T__1);
    setState(21);
    match(ifccParser::T__2);
    setState(22);
    match(ifccParser::T__3);
    setState(23);
    match(ifccParser::T__4);
    setState(27);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 75497506) != 0)) {
      setState(24);
      stmt();
      setState(29);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(30);
    match(ifccParser::T__5);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StmtContext ------------------------------------------------------------------

ifccParser::StmtContext::StmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ifccParser::Return_stmtContext* ifccParser::StmtContext::return_stmt() {
  return getRuleContext<ifccParser::Return_stmtContext>(0);
}

ifccParser::AssignContext* ifccParser::StmtContext::assign() {
  return getRuleContext<ifccParser::AssignContext>(0);
}

ifccParser::DeclarContext* ifccParser::StmtContext::declar() {
  return getRuleContext<ifccParser::DeclarContext>(0);
}

ifccParser::BlockContext* ifccParser::StmtContext::block() {
  return getRuleContext<ifccParser::BlockContext>(0);
}


size_t ifccParser::StmtContext::getRuleIndex() const {
  return ifccParser::RuleStmt;
}


std::any ifccParser::StmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitStmt(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::StmtContext* ifccParser::stmt() {
  StmtContext *_localctx = _tracker.createInstance<StmtContext>(_ctx, getState());
  enterRule(_localctx, 4, ifccParser::RuleStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(40);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ifccParser::T__0:
      case ifccParser::RETURN:
      case ifccParser::ID: {
        enterOuterAlt(_localctx, 1);
        setState(35);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case ifccParser::RETURN: {
            setState(32);
            return_stmt();
            break;
          }

          case ifccParser::ID: {
            setState(33);
            assign();
            break;
          }

          case ifccParser::T__0: {
            setState(34);
            declar();
            break;
          }

        default:
          throw NoViableAltException(this);
        }
        setState(37);
        match(ifccParser::T__6);
        break;
      }

      case ifccParser::T__4: {
        enterOuterAlt(_localctx, 2);
        setState(39);
        block();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Return_stmtContext ------------------------------------------------------------------

ifccParser::Return_stmtContext::Return_stmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ifccParser::Return_stmtContext::RETURN() {
  return getToken(ifccParser::RETURN, 0);
}

ifccParser::ExprContext* ifccParser::Return_stmtContext::expr() {
  return getRuleContext<ifccParser::ExprContext>(0);
}


size_t ifccParser::Return_stmtContext::getRuleIndex() const {
  return ifccParser::RuleReturn_stmt;
}


std::any ifccParser::Return_stmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitReturn_stmt(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::Return_stmtContext* ifccParser::return_stmt() {
  Return_stmtContext *_localctx = _tracker.createInstance<Return_stmtContext>(_ctx, getState());
  enterRule(_localctx, 6, ifccParser::RuleReturn_stmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(42);
    match(ifccParser::RETURN);
    setState(43);
    expr(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssignContext ------------------------------------------------------------------

ifccParser::AssignContext::AssignContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ifccParser::AssignContext::ID() {
  return getToken(ifccParser::ID, 0);
}

ifccParser::ExprContext* ifccParser::AssignContext::expr() {
  return getRuleContext<ifccParser::ExprContext>(0);
}


size_t ifccParser::AssignContext::getRuleIndex() const {
  return ifccParser::RuleAssign;
}


std::any ifccParser::AssignContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitAssign(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::AssignContext* ifccParser::assign() {
  AssignContext *_localctx = _tracker.createInstance<AssignContext>(_ctx, getState());
  enterRule(_localctx, 8, ifccParser::RuleAssign);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(45);
    match(ifccParser::ID);
    setState(46);
    match(ifccParser::T__7);
    setState(47);
    expr(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DeclarContext ------------------------------------------------------------------

ifccParser::DeclarContext::DeclarContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> ifccParser::DeclarContext::ID() {
  return getTokens(ifccParser::ID);
}

tree::TerminalNode* ifccParser::DeclarContext::ID(size_t i) {
  return getToken(ifccParser::ID, i);
}


size_t ifccParser::DeclarContext::getRuleIndex() const {
  return ifccParser::RuleDeclar;
}


std::any ifccParser::DeclarContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitDeclar(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::DeclarContext* ifccParser::declar() {
  DeclarContext *_localctx = _tracker.createInstance<DeclarContext>(_ctx, getState());
  enterRule(_localctx, 10, ifccParser::RuleDeclar);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(49);
    match(ifccParser::T__0);
    setState(50);
    match(ifccParser::ID);
    setState(55);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ifccParser::T__8) {
      setState(51);
      match(ifccParser::T__8);
      setState(52);
      match(ifccParser::ID);
      setState(57);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlockContext ------------------------------------------------------------------

ifccParser::BlockContext::BlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ifccParser::StmtContext *> ifccParser::BlockContext::stmt() {
  return getRuleContexts<ifccParser::StmtContext>();
}

ifccParser::StmtContext* ifccParser::BlockContext::stmt(size_t i) {
  return getRuleContext<ifccParser::StmtContext>(i);
}


size_t ifccParser::BlockContext::getRuleIndex() const {
  return ifccParser::RuleBlock;
}


std::any ifccParser::BlockContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitBlock(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::BlockContext* ifccParser::block() {
  BlockContext *_localctx = _tracker.createInstance<BlockContext>(_ctx, getState());
  enterRule(_localctx, 12, ifccParser::RuleBlock);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(58);
    match(ifccParser::T__4);
    setState(62);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 75497506) != 0)) {
      setState(59);
      stmt();
      setState(64);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(65);
    match(ifccParser::T__5);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprContext ------------------------------------------------------------------

ifccParser::ExprContext::ExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t ifccParser::ExprContext::getRuleIndex() const {
  return ifccParser::RuleExpr;
}

void ifccParser::ExprContext::copyFrom(ExprContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- ExprUnaryNotContext ------------------------------------------------------------------

ifccParser::ExprContext* ifccParser::ExprUnaryNotContext::expr() {
  return getRuleContext<ifccParser::ExprContext>(0);
}

ifccParser::ExprUnaryNotContext::ExprUnaryNotContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ifccParser::ExprUnaryNotContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitExprUnaryNot(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprEqContext ------------------------------------------------------------------

std::vector<ifccParser::ExprContext *> ifccParser::ExprEqContext::expr() {
  return getRuleContexts<ifccParser::ExprContext>();
}

ifccParser::ExprContext* ifccParser::ExprEqContext::expr(size_t i) {
  return getRuleContext<ifccParser::ExprContext>(i);
}

ifccParser::ExprEqContext::ExprEqContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ifccParser::ExprEqContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitExprEq(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprMultContext ------------------------------------------------------------------

std::vector<ifccParser::ExprContext *> ifccParser::ExprMultContext::expr() {
  return getRuleContexts<ifccParser::ExprContext>();
}

ifccParser::ExprContext* ifccParser::ExprMultContext::expr(size_t i) {
  return getRuleContext<ifccParser::ExprContext>(i);
}

ifccParser::ExprMultContext::ExprMultContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ifccParser::ExprMultContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitExprMult(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprCmpContext ------------------------------------------------------------------

std::vector<ifccParser::ExprContext *> ifccParser::ExprCmpContext::expr() {
  return getRuleContexts<ifccParser::ExprContext>();
}

ifccParser::ExprContext* ifccParser::ExprCmpContext::expr(size_t i) {
  return getRuleContext<ifccParser::ExprContext>(i);
}

ifccParser::ExprCmpContext::ExprCmpContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ifccParser::ExprCmpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitExprCmp(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprBitAndContext ------------------------------------------------------------------

std::vector<ifccParser::ExprContext *> ifccParser::ExprBitAndContext::expr() {
  return getRuleContexts<ifccParser::ExprContext>();
}

ifccParser::ExprContext* ifccParser::ExprBitAndContext::expr(size_t i) {
  return getRuleContext<ifccParser::ExprContext>(i);
}

ifccParser::ExprBitAndContext::ExprBitAndContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ifccParser::ExprBitAndContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitExprBitAnd(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprUnaryMinusContext ------------------------------------------------------------------

ifccParser::ExprContext* ifccParser::ExprUnaryMinusContext::expr() {
  return getRuleContext<ifccParser::ExprContext>(0);
}

ifccParser::ExprUnaryMinusContext::ExprUnaryMinusContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ifccParser::ExprUnaryMinusContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitExprUnaryMinus(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprParenContext ------------------------------------------------------------------

ifccParser::ExprContext* ifccParser::ExprParenContext::expr() {
  return getRuleContext<ifccParser::ExprContext>(0);
}

ifccParser::ExprParenContext::ExprParenContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ifccParser::ExprParenContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitExprParen(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprAddContext ------------------------------------------------------------------

std::vector<ifccParser::ExprContext *> ifccParser::ExprAddContext::expr() {
  return getRuleContexts<ifccParser::ExprContext>();
}

ifccParser::ExprContext* ifccParser::ExprAddContext::expr(size_t i) {
  return getRuleContext<ifccParser::ExprContext>(i);
}

ifccParser::ExprAddContext::ExprAddContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ifccParser::ExprAddContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitExprAdd(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprBitXorContext ------------------------------------------------------------------

std::vector<ifccParser::ExprContext *> ifccParser::ExprBitXorContext::expr() {
  return getRuleContexts<ifccParser::ExprContext>();
}

ifccParser::ExprContext* ifccParser::ExprBitXorContext::expr(size_t i) {
  return getRuleContext<ifccParser::ExprContext>(i);
}

ifccParser::ExprBitXorContext::ExprBitXorContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ifccParser::ExprBitXorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitExprBitXor(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprIdContext ------------------------------------------------------------------

tree::TerminalNode* ifccParser::ExprIdContext::ID() {
  return getToken(ifccParser::ID, 0);
}

ifccParser::ExprIdContext::ExprIdContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ifccParser::ExprIdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitExprId(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprBitOrContext ------------------------------------------------------------------

std::vector<ifccParser::ExprContext *> ifccParser::ExprBitOrContext::expr() {
  return getRuleContexts<ifccParser::ExprContext>();
}

ifccParser::ExprContext* ifccParser::ExprBitOrContext::expr(size_t i) {
  return getRuleContext<ifccParser::ExprContext>(i);
}

ifccParser::ExprBitOrContext::ExprBitOrContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ifccParser::ExprBitOrContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitExprBitOr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprConstContext ------------------------------------------------------------------

tree::TerminalNode* ifccParser::ExprConstContext::CONST() {
  return getToken(ifccParser::CONST, 0);
}

ifccParser::ExprConstContext::ExprConstContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ifccParser::ExprConstContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitExprConst(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprCharConstContext ------------------------------------------------------------------

tree::TerminalNode* ifccParser::ExprCharConstContext::CHAR_CONST() {
  return getToken(ifccParser::CHAR_CONST, 0);
}

ifccParser::ExprCharConstContext::ExprCharConstContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ifccParser::ExprCharConstContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitExprCharConst(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::ExprContext* ifccParser::expr() {
   return expr(0);
}

ifccParser::ExprContext* ifccParser::expr(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  ifccParser::ExprContext *_localctx = _tracker.createInstance<ExprContext>(_ctx, parentState);
  ifccParser::ExprContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 14;
  enterRecursionRule(_localctx, 14, ifccParser::RuleExpr, precedence);

    size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(79);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ifccParser::T__9: {
        _localctx = _tracker.createInstance<ExprUnaryMinusContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(68);
        match(ifccParser::T__9);
        setState(69);
        expr(13);
        break;
      }

      case ifccParser::T__10: {
        _localctx = _tracker.createInstance<ExprUnaryNotContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(70);
        match(ifccParser::T__10);
        setState(71);
        expr(12);
        break;
      }

      case ifccParser::T__2: {
        _localctx = _tracker.createInstance<ExprParenContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(72);
        match(ifccParser::T__2);
        setState(73);
        expr(0);
        setState(74);
        match(ifccParser::T__3);
        break;
      }

      case ifccParser::CONST: {
        _localctx = _tracker.createInstance<ExprConstContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(76);
        match(ifccParser::CONST);
        break;
      }

      case ifccParser::CHAR_CONST: {
        _localctx = _tracker.createInstance<ExprCharConstContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(77);
        match(ifccParser::CHAR_CONST);
        break;
      }

      case ifccParser::ID: {
        _localctx = _tracker.createInstance<ExprIdContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(78);
        match(ifccParser::ID);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(104);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(102);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<ExprMultContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(81);

          if (!(precpred(_ctx, 7))) throw FailedPredicateException(this, "precpred(_ctx, 7)");
          setState(82);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & 28672) != 0))) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(83);
          expr(8);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<ExprAddContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(84);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(85);
          _la = _input->LA(1);
          if (!(_la == ifccParser::T__9

          || _la == ifccParser::T__14)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(86);
          expr(7);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<ExprCmpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(87);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(88);
          _la = _input->LA(1);
          if (!(_la == ifccParser::T__15

          || _la == ifccParser::T__16)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(89);
          expr(6);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<ExprEqContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(90);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(91);
          _la = _input->LA(1);
          if (!(_la == ifccParser::T__17

          || _la == ifccParser::T__18)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(92);
          expr(5);
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<ExprBitAndContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(93);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(94);
          match(ifccParser::T__19);
          setState(95);
          expr(4);
          break;
        }

        case 6: {
          auto newContext = _tracker.createInstance<ExprBitXorContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(96);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(97);
          match(ifccParser::T__20);
          setState(98);
          expr(3);
          break;
        }

        case 7: {
          auto newContext = _tracker.createInstance<ExprBitOrContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(99);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(100);
          match(ifccParser::T__21);
          setState(101);
          expr(2);
          break;
        }

        default:
          break;
        } 
      }
      setState(106);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

bool ifccParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 7: return exprSempred(antlrcpp::downCast<ExprContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool ifccParser::exprSempred(ExprContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 7);
    case 1: return precpred(_ctx, 6);
    case 2: return precpred(_ctx, 5);
    case 3: return precpred(_ctx, 4);
    case 4: return precpred(_ctx, 3);
    case 5: return precpred(_ctx, 2);
    case 6: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

void ifccParser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  ifccParserInitialize();
#else
  ::antlr4::internal::call_once(ifccParserOnceFlag, ifccParserInitialize);
#endif
}
