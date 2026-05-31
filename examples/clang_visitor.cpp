// Source - https://stackoverflow.com/a/77839521
// Posted by Scott McPeak, modified by community. See post 'Timeline' for change history
// Retrieved 2026-05-31, License - CC BY-SA 4.0

bool Visitor::VisitFunctionDecl(clang::FunctionDecl *functionDecl)
{
  // Get the function start and end.  If either arises from a macro
  // expansion, get the location of that expansion, not the place in
  // the macro definition where the token was spelled.
  clang::SourceManager &sm = m_astContext.getSourceManager();
  clang::SourceLocation beginLoc = sm.getExpansionLoc(functionDecl->getBeginLoc());
  clang::SourceLocation endLoc = sm.getExpansionLoc(functionDecl->getEndLoc());

  // Print some details about the function.
  cout << functionDecl->Decl::getDeclKindName()
       << " \"" << functionDecl->getQualifiedNameAsString()
       << "\" type=\"" << typeStr(functionDecl->getType())
       << "\" at "
       << declLocStr(functionDecl)
       << ": beginLoc=" << locStr(beginLoc)
       << ", endLoc=" << locStr(endLoc)
       << "\n";

  // Get the memory buffer containing the source code.
  clang::FileID fileID = sm.getFileID(beginLoc);
  std::optional<llvm::MemoryBufferRef> buffer = sm.getBufferOrNone(fileID);
  if (!buffer) {
    // I haven't seen this happen, but might be possible when the input
    // is a serialized AST file rather than source code.
    cout << "  No buffer for this source file.\n";
    return true;
  }

  // Range of text to lex.
  char const *fileStartPtr = buffer->getBufferStart();
  char const *functionStartPtr = fileStartPtr + sm.getFileOffset(beginLoc);
  char const *functionEndPtr = fileStartPtr + sm.getFileOffset(endLoc);

  // Prepare to lex it in "raw" mode, meaning no preprocessing is done.
  clang::Lexer lexer(
    beginLoc,
    m_astContext.getLangOpts(),
    functionStartPtr,
    functionStartPtr,
    functionEndPtr);

  // Iterate over all of the tokens in the function definition range.
  // Do not rely only on the return value from 'LexFromRawLexer' to know
  // when to stop because, depending on where the end is relative to
  // tokens, it can skip over the specified end and read into unrelated
  // memory and eventually segfault (Clang bug?).
  clang::Token token;
  while (lexer.getBufferLocation() < functionEndPtr &&
         lexer.LexFromRawLexer(token /*OUT*/) == false) {
    cout << "  " << locStr(token.getLocation())
         << ": kind=" << token.getName()
         << ", len=" << token.getLength()
         << ", text: \""
         << std::string(
              fileStartPtr + sm.getFileOffset(token.getLocation()),
              token.getLength())
         << "\"\n";
  }

  return true;
}
