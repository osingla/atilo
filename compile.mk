.PHONY: all clean

all: $(TARGET)

$(TARGET):	$(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIBS) $(LDFLAGS)

-include $(OBJS:.o=.d)

clean:
	@rm -rf *.o *.d *.err $(TARGET)
