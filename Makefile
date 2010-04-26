NAME		=	zia
SRC_DIR		=	./src/
TOOL_DIR	=	$(SRC_DIR)tools/
COM_DIR		=	$(SRC_DIR)common/

SRC		=	$(COM_DIR)isdir.cpp		\
			$(COM_DIR)splitstring.cpp	\
			$(COM_DIR)struppercase.cpp	\
			$(COM_DIR)strlowercase.cpp	\
			$(COM_DIR)inttostr.cpp		\
			$(COM_DIR)urldecode.cpp		\
			$(COM_DIR)urlencode.cpp		\
			$(COM_DIR)directoryList.cpp	\
			$(TOOL_DIR)IULibrary.cpp	\
			$(TOOL_DIR)IUSocket.cpp		\
			$(TOOL_DIR)IUThread.cpp		\
			$(TOOL_DIR)IUFileSystem.cpp	\
			$(SRC_DIR)main.cpp		\
			$(SRC_DIR)Zia.cpp		\
			$(SRC_DIR)Server.cpp		\
			$(SRC_DIR)Client.cpp		\
			$(SRC_DIR)Parser.cpp		\
			$(SRC_DIR)ResponseBuilder.cpp	\
			$(SRC_DIR)ModuleManager.cpp	\
			$(SRC_DIR)Hooker.cpp		\
			$(SRC_DIR)Conf.cpp		\
			$(SRC_DIR)AdminConsole.cpp

OBJ		=	$(SRC:.cpp=.o)
CC		=	g++
CFLAGS		=	-W -Wall
LFLAGS		=	-lpthread -ldl -lQtXml -lQtCore


all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(LFLAGS) $(CFLAGS)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

etags:  #for emacs, create the TAGS file in src/
	find . -name "*pp" -type f | xargs etags -a


.cpp.o:
	g++ $(CFLAGS) $(IFLAGS) -c $< -o $@  -I/usr/include/QtCore -I/usr/include/QtXml -Isrc/api -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtXml -I/usr/include/qt4/

modlogger:
	cd Modules/ModLogger/ && make && cd ../../ && ((cp Modules/ModLogger/ModLogger.so modules/) || (mkdir modules && cp Modules/ModLogger/ModLogger.so modules/))


modopenssl:
	cd Modules/ModOpenSSL/ && make && cd ../../ && ((cp Modules/ModOpenSSL/ModOpenSSL.so modules/) || (mkdir modules && cp Modules/ModOpenSSL/ModOpenSSL.so modules/))

modphp:
	cd Modules/ModPhp/ && make && cd ../../ && ((cp Modules/ModPhp/ModPhp.so modules/) || (mkdir modules && cp Modules/ModPhp/ModPhp.so modules/))

