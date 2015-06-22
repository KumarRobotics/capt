function hpatch = circle(p, r, c, o)
% hpatch = circle(p, r, c)
%   circle draws circles of radius r and color c at the locations
%   determined by the matrix p. The first column of p is the x coordinates
%   of the circles and the second is the y coordinates.
angle = 0.0:0.01:2*pi;
xp = r*cos(angle);
yp = r*sin(angle);

X = bsxfun(@plus, p(:, 1)', xp');
Y = bsxfun(@plus, p(:, 2)', yp');

hpatch = patch(X, Y, c, 'FaceAlpha', o);


end

