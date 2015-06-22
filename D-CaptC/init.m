function init(start, goal, traj)
% init(start, goal, traj)
%   init creates a new plot and draws the start locations, goals and
%   trajectories

newplot;
circle(start, 0.1, [1, 0, 0], 1.0);
circle(goal, 0.1, [1, 1 , 1], 1.0);
[N, ~] = size(start);



for n = 1:N
    line(traj{n}(:, 1), traj{n}(:, 2));
end

axis equal
axis([-2, 7, -2, 7]);

end

