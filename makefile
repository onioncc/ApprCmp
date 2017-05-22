
TOP		= ..

#LEDA_INC	=	/usr/local/share/LEDA-6.2/incl/
#LEDA_LIB	=   /usr/local/share/LEDA-6.2/
#CBC_L=/home/zhongwei/Cbc/lib
#CBC_I=/home/zhongwei/Cbc/include

# Target
TARGET		=	./result

# Sources
SOURCES	=	$(wildcard *.c)

OBJPATH		= ../obj/
OBJECTS		= $(SOURCES:%.c=$(OBJPATH)%.o)

LIBFLAGS	=\
 		-lcurses -ltermcap \
		-lm \
		-pg #./liblpsolve55.a

# Compilation MACROs
INCFLAGS = -I$(TOP)/inc/ -I$(HOME)/include -I$(LEDA_INC) -I$(CBC_I)
#CFLAGS   = -Wno-deprecated  -O3
CFLAGS   = -Wno-deprecated  -O3

CC      	= gcc $(CFLAGS)
CXX     	= g++ $(CFLAGS)
LINK    	= g++ $(LFLAGS)

# Rule definition
$(TARGET): $(OBJECTS)
	@echo "Linking $@ ... "
	@$(LINK) -pg -o $@ $(OBJECTS) $(LIBFLAGS) 
	@echo done

$(OBJECTS):	$(OBJPATH)%.o:%.c 
	@echo "Compiling $(@F:.o=.c) ... "
	@$(CXX) -pg -c $(INCFLAGS) $< -o $@
#	@$(CXX)  -c $(INCFLAGS) $< -o $@

# Make debug
 
debug:
	@make CFLAGS="-g -pg -D_DEBUG -D_ALP -Wall -Wno-deprecated -Wno-unused -Wunreachable-code" LFLAGS=""

clean:
	@echo Removing $(OBJECTS)
	@\rm $(OBJECTS) 
.KEEP_STATE:

%.d: %.c
	set -e; $(CXX) -M $(CPPFLAGS) $(INCFLAGS) $< \
	| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
	[ -s $@ ] || rm -f $@

#include $(SOURCES:%.c=%.d)

docs:
	@echo Making Docs ......
	@doxygen Doxyfile
	
