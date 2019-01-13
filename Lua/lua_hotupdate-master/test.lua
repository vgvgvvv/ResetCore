local test = {}
test.count = 0
local count = 0
local d_count = 0
function test.func()
    count = count + 1
    d_count = d_count + 2
    test.count = test.count + 3
    print("test 333", count, d_count, test.count)
    return true
end
return test
