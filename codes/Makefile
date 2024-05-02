OUT = projet
CXX     = g++
CXXFLAGS =-g -Wall -std=c++11
LINKING = `pkg-config --cflags gtkmm-3.0`
LDLIBS = `pkg-config --libs gtkmm-3.0`
CXXFILES = projet.cc simulation.cc gisement.cc base.cc robot.cc geomod.cc message.cc gui.cc graphic.cc
OFILES= projet.o simulation.o gisement.o base.o robot.o geomod.o message.o gui.o graphic.o

all : $(OUT)

graphic.o: graphic.cc graphic.h graphic_gui.h constantes.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
	
gui.o: gui.cc gui.h graphic.h simulation.h geomod.h graphic_gui.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

message.o: message.cc message.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

geomod.o: geomod.cc geomod.h graphic.h graphic_gui.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
	
simulation.o: simulation.cc simulation.h gisement.h base.h geomod.h 
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
	
gisement.o: gisement.cc gisement.h message.h geomod.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
	
robot.o: robot.cc robot.h gisement.h message.h geomod.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
		
base.o: base.cc base.h gisement.h robot.h geomod.h message.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
	
projet.o: projet.cc simulation.h gui.h 
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

$(OUT): $(OFILES)
	$(CXX) $(CXXFLAGS) $(LINKING) $(OFILES) -o $@ $(LDLIBS)


depend:
	@echo " *** MISE A JOUR DES DEPENDANCES ***"
	@(sed '/^# DO NOT DELETE THIS LINE/q' Makefile && \
	  $(CXX) -MM $(CXXFLAGS) $(CXXFILES) | \
	  egrep -v "/usr/include" \
	 ) >Makefile.new
	@mv Makefile.new Makefile

clean:
	@echo " *** EFFACE MODULES OBJET ET EXECUTABLE ***"
	@rm *.o $(OUT) *.cc~ *.h~

#~ projet.o: projet.cc geomod.h message_geomod.h
#~ geomod.o: geomod.cc geomod.h
#~ message_geomod.o: message_geomod.cc message_geomod.h

#OUT = gui
# CXX = g++
#CXXFLAGS = -Wall -std=c++11 
#LINKING = `pkg-config --cflags gtkmm-3.0`
#LDLIBS = `pkg-config --libs gtkmm-3.0`
#OFILES = main.o gui.o 

#all: $(OUT)

#~ gui.o: gui.cc gui.h
#~ 	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

#~ main.o: main.cc gui.h
#~ 	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

#~ $(OUT): $(OFILES)
#~ 	$(CXX) $(CXXFLAGS) $(LINKING) $(OFILES) -o $@ $(LDLIBS)

#~ clean:
#~ 	@echo "Cleaning compilation files"
#~ 	@rm *.o $(OUT) *.cc~ *.h~
