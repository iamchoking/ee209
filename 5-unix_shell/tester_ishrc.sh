./sampleish > out_test_sampleish 2>err_test_sampleish
./ish > out_test_ish 2>err_test_ish

diff out_test_sampleish out_test_ish
diff err_test_sampleish err_test_ish