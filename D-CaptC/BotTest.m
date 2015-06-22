

master = BotMaster(1);

bot1 = Bot(1.0, [1, 1], [5, 3], master);
bot2 = Bot(1.0, [0, 0], [2, 4], master);
bot3 = Bot(1.0, [1, 2], [1, 3], master);
bot4 = Bot(1.0, [0, 3], [1, 0], master);

bots = [bot1, bot2, bot3, bot4];
bot1alias = bot1;



for j = 0:0.1:1
    
   %newplot;
   for i = 1:4
    bots(i).step(j);
    
   end 
   
  
   %circle(pos, 0.1, [1, 0, 0]);
   axis equal;
end

bot1.traveled

for i = 1:length(bot1.timeSweep)
    pos = [bot1.traveled(i, :); bot2.traveled(i, :); bot3.traveled(i, :); bot4.traveled(i, :)];
    circle(pos, 0.1, [1, 0, 0], 1.0);
end
bot1.pos(1)
bot2.pos(1)
bot3.pos(1)
bot4.pos(1)
clear;
