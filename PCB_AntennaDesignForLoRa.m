%% Create Variable
l1 = 3.94e-3;
l2 = 2.70e-3;
l3 = 5e-3;
l4 = 2.64e-3;
l5 = 2e-3;
l6 = 4.90e-3;

w1 = 0.90e-3;
w2 = 0.5e-3;

d4 = 0.5e-3;
d5 = 1.4e-3;
d6 = 1.70e-3;

lgp = 15.2e-3; % Length of Groundplane
wgp = 30e-3;   % Width of Groundplane

lbs = 15.2e-3; % Length for the Board Shape
wbs = 5.2e-3;  % Width for the Board Shape

%% Create Geometry
a = antenna.Rectangle('Length',w1,'Width',l6,'Center',[w1/2,l6/2-d4]);
b = antenna.Rectangle('Length',l3,'Width',w2,'Center',[l3/2,w2/2+l6-d4]);
c = antenna.Rectangle('Length',w2,'Width',l6,'Center',[w2/2+d5+w1,l6/2-d4]);
d = antenna.Rectangle('Length',w2,'Width',l4,'Center',[w2/2+w1+d5+w2+d6,-l4/2+l6-d4]);
e = antenna.Rectangle('Length',l5,'Width',w2,'Center',[l5/2+l3,-w2/2+l6-l4]);
f = antenna.Rectangle('Length',w2,'Width',l4,'Center',[w2/2+w1+d5+w2+d6+w2+l5,-l4/2+l6-d4]);
g = antenna.Rectangle('Length',l2,'Width',w2,'Center',[l2/2+w1+d5+w2+d6+w2+l5,w2/2+l6-d4]);
h = antenna.Rectangle('Length',w2,'Width',l4,'Center',[w2/2+w1+d5+d6+w2+l5+l2,-l4/2+l6-d4]);
i = antenna.Rectangle('Length',l5,'Width',w2,'Center',[l5/2+l3+l5+l2,-w2/2+l6-l4]);
j = antenna.Rectangle('Length',w2,'Width',l4,'Center',[w2/2+w1+d5+w2+d6+w2+l5+l2+l5,-l4/2+l6-d4]);
k = antenna.Rectangle('Length',l2,'Width',w2,'Center',[l2/2+w1+d5+w2+d6+w2+l5+l2+l5,w2/2+l6-d4]);
l = antenna.Rectangle('Length',w2,'Width',l1,'Center',[w1+d5+w2+d6+w2/2+l5+l2+l5+l2,-l1/2+l6-d4]);

antennaLoRa = a+b+c+d+e+f+g+h+i+j+k+l;
    

%% PCB Board Design

d = dielectric('FR4');
d.Thickness = 0.25e-3;
d.EpsilonR = 4.5;
d1 = dielectric('FR4');
d1.Thickness = 0.75e-3;
d1.EpsilonR = 4.5;
ant = pcbStack;
boardshape = antenna.Rectangle('Length',lbs,'Width',wbs+wgp,'Center',[lbs/2-0.5e-3,(wbs-wgp)/2]);
ant.BoardShape = boardshape;
ant.BoardThickness = d.Thickness + d1.Thickness;
gnd = antenna.Rectangle('Length',lgp,'Width',wgp,'Center',[lgp/2-0.5e-3,-wgp/2]);
ant.Layers = {antennaLoRa,d,gnd,d1};
ant.FeedDiameter = w2/2;
ant.ViaDiameter = w1/4;
ant.FeedLocations = [w1+1.4e-3+w2/2,-d4/2,3,1];
ant.FeedViaModel = 'square';
ant.Conductor = metal('Copper');
ant.Conductor.Thickness = 35e-6;
figure;
show(ant)
hold
%% Analyze Antenna Performance
spar = sparameters(ant,linspace(2e9,3e9,21));
figure;
rfplot(spar);
hold

%show mesh with MaxEdgeLength
figure;
mesh(ant,'MaxEdgeLength',1.2e-3)
hold

%after improve mesh
spar = sparameters(ant,linspace(2.2e9,2.7e9,11));
figure;
rfplot(spar)
hold

%3d plot
figure;
pattern(ant,2.4e9)
hold
