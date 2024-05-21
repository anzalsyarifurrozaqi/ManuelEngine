# root directory of the project
DIR=$(dirname "$(readlink -f "$-1")")

MODULE_NAME=$1
# get the name of the project crom the CMakeLists.txt file
PROJECT_NAME=$(grep "project(\w*" CMakeLists.txt -o | grep "(.*" -o | cut -c 2-)

if [ -d $DIR/${PROJECT_NAME}/${MODULE_NAME} ]; then
    echo "The module already exists"
    exit 1
fi

echo ${PROJECT_NAME} ${MODULE_NAME} ${DIR}

# copy the template module and rename its parts
cp -r $DIR/Templates/TemplateSrcModule/ $DIR/src/
mv $DIR/src/TemplateSrcModule/ $DIR/src/${MODULE_NAME}
mv $DIR/src/${MODULE_NAME}/include/MODULE_NAME $DIR/src/${MODULE_NAME}/include/${MODULE_NAME}

# expand 'MODULE_NAME' to the name of the module in every file of the module's folder
find $DIR/src/${MODULE_NAME} -type f -exec sed -i "s/MODULE_NAME/${MODULE_NAME}/g" {} \;

# add the module to the 'src/CMakeLists.txt' script
LINE="add_subdirectory(${MODULE_NAME})"
if [ ! $(grep $LINE $DIR/src/CMakeLists.txt) ]; then
    echo ${LINE} >> $DIR/src/CMakeLists.txt
fi