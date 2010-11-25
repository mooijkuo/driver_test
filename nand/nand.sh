#! /bin/bash

echo ""
echo "========= test nand =========="
echo ""
echo "====== erase nand ======"
./nand/flash_eraseall /dev/mtd4
echo "== write data to nand==="
./nand/nandwrite -p /dev/mtd4 ./nand/nand128k
echo "=== read data to tmp ==="
./nand/mtd_debug read /dev/mtd4 0x0 0x20000 /tmp/read128k
echo "===== compare data ====="
diff -s ./nand/nand128k /tmp/read128k
echo ""
if [ $? == 0 ]; then
	echo "nand test pass!"
else
	echo "nand test fail!"
fi
echo ""
rm /tmp/read128k
echo "========== test end =========="
echo ""