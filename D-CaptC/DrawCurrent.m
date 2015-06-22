function DrawCurrent(hObject, start, goal, traj)
% DrawCurrent(hObject, start, goal, traj)
%   DrawCurrent refreshes the plot and draws the position of the robots at
%   a time determined by the state of the slider
init(start, goal, traj);

n = get(hObject,'Value');
[s, ~] = size(traj{1});
t = max([fix(n*s), 1]);

[~, N] = size(traj);

Pos = traj{1}(t, :);

for i = 2:N
    Pos = [Pos;traj{i}(t, :)];
   
end


circle(Pos, 0.6, [0, 1, 0], 0.25);
circle(Pos, 0.2, [0, 0, 0], 1.0);


