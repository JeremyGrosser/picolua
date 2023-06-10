function px(addr)
    io.write(string.format("%02x ", peek(addr)))
    io.flush()
end

CRLF = string.char(13) .. string.char(10)

function dump(addr, length)
  if length == nil then
    length = 64
  end
  for i = 0, length-1 do
      if i % 16 == 0 then
          io.write(CRLF)
          io.write(string.format("%08x: ", addr + i))
      end
      px(addr + i)
  end
  io.write(CRLF)
  io.flush()
end
