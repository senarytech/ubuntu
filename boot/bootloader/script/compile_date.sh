pwd
echo "char *compile_date=\"binary build time: `date`\";" > include/date.h
echo "char *sdk_revision=\"$SDK_RELEASE_BUILD_NUMBER\";" >> include/date.h
cat include/date.h
