include $(INCLUDE)

LIB = ../lib/febioxml_$(PLAT).a

$(LIB):
	$(CC) -c $(INC) $(DEF) $(FLG) *.cpp
	ar -cvr $(LIB) *.o

clean:
	rm -f *.o
	rm -f $(LIB)
