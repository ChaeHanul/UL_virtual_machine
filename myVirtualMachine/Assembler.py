# -*- coding: cp949 -*-
import sys
type0 = {"ADD":0x01,"SUB":0x02,"MUL":0x03,"DIV":0x04,"OR":0x05,"AND":0x06,"XOR":0x07,"SHL":0x08,"SHR":0x09,"CMP":0x0A}
type1 = {"STORE":0x21,"LOAD":0x22,"INC":0x23,"DEC":0x24,"NOT":0x25}
type2 = {"B":0x48,"BEQ":0x49,"BNE":0x4A,"BL":0x4B,"BG":0x4C}
instructions = [type0,type1,type2]
register = []
for num in range(13):
    register.append("r"+str(num))
path = input("input you *.ua >>> ")
ext = path.split(".")[1]
if ext != "ua" :
    print("it is not *.ua")
    sys.exit()

##파싱 루틴
def file_parser(my_file):
    string_data = my_file.read()
    result = []
    parsing_data = string_data.split("\n")
    for parsing_data_item in parsing_data :
        if check_instruction(parsing_data_item) == False:
            print(parsing_data_item + ">> this line error !!")
            sys.exit()
        result.append(parsing_data_item.split("\t"))
    return result
##검사 루틴
##error 0 : 명령어 들어가야 할 줄에 명령어 없음
##error 1 : 인자 갯수 안 맞음
##error 2 : 인자 갯수는 맞으나 해당 레지스터는 존재하지않음 (오타)
def check_instruction(parsing_data_item):
    instruction = parsing_data_item.split("\t")
    for typeX in instructions:
        if instruction[0] in typeX:
            if typeX == type0:
                if len(instruction) == 3:
                    if register.count(instruction[1]) == 0 or register.count(instruction[2]) == 0:
                        print("error2")
                        return False
                    return True
                print("error1")
                return False
            elif typeX == type1:
                if len(instruction) == 2:
                    if register.count(instruction[1]) == 0 :
                        print("error2")
                        return False
                    return True
                print("error1")
                return False
            else:
                if len(instruction) == 2:
                    if register.count(instruction[1]) == 0 :
                        print("error2")
                        return False
                    return True
                print("error1")
                return False
    print("error0")
    return False
##검사후 인코드 루틴
def instruction_decode(file_parser_result):
    bin_file = open("test.bin","wb")
    for item in file_parser_result:
        write_data = b''
        opHead = 0xFF
        if len(item) == 2:
            reg_num = register.index(item[1])
            if item[0] in type1:
                reg_num = 0xff & (reg_num << 4)
                opHead = type1[item[0]]
            else:
                opHead = type2[item[0]]
            opBody = reg_num
            print(hex(opHead))
            write_data = write_data + bytes([opHead]) + bytes([opBody])
        else:
            reg_num1 = register.index(item[1]) << 4
            reg_num2 = register.index(item[2])
            opHead = type0[item[0]]
            opBody = 0xff&(reg_num1|reg_num2)
            print(hex(opHead))
            write_data = write_data + bytes([opHead]) + bytes([opBody])
        bin_file.write(write_data)
    bin_file.close()

if __name__ == "__main__":
    my_file = open(path,"r")
    file_parser_result = file_parser(my_file)
    instruction_decode(file_parser_result)

