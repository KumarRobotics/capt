function phi = CreatePhi(perm_idx_row)


perm_idx_row = perm_idx_row(:);

n = size(perm_idx_row,1);
phi = zeros(n,n);
for ii = 1:n
    phi(ii,perm_idx_row(ii)) = 1;
end

assert(isequal(ones(1,n)*phi,ones(1,n)));
assert(isequal(phi*ones(n,1),ones(n,1)));