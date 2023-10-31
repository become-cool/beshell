#! /usr/bin/fish

if not make
    exit
end

echo "-----"
echo ""

while true

    rlwrap build/beshell
    if test $status -ne 99
        exit
    end

end
