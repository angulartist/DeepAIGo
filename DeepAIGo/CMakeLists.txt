add_library(DeepAIGo board/Board.cc gtp/AIGoGtp.cc gtp/GtpEngine.cc
                    io/SgfParser.cc net/helper.cc net/PolicyNet.cc
                    preprocess/Preprocess.cc preprocess/Symmetrics.cc
                    mcts/MCTSEngine.cc mcts/TreeNode.cc)
target_link_libraries (DeepAIGo ${CMAKE_THREAD_LIBS_INIT})