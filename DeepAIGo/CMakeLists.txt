add_library(DeepAIGo board/Board.cc gtp/AIGoGtp.cc gtp/GtpEngine.cc
                    io/SgfParser.cc net/helper.cc net/PolicyNet.cc
                    preprocess/Preprocess.cc)