//
//  main.c
//  z-wave-test
//
//  Created by Alex Skalozub on 1/6/12.
//  Copyright (c) 2012 pieceofsummer@gmail.com. All rights reserved.
//

#include <stdio.h>
#include <ZWayLib.h>
#include <ZLogging.h>
#include <ZDataPublic.h>
#include <errno.h>

int do_work(ZWay zway);

void test_memory(ZWay zway)
{
    ZWError r;
    
    ZWBYTE buff1[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    
    r = zway_fc_memory_put_byte(zway, 0x1234, 0x42, NULL, NULL, NULL);
    r = zway_fc_memory_get_byte(zway, 0x1234, NULL, NULL, NULL);
    r = zway_fc_memory_put_buffer(zway, 0x2345, 16, buff1, NULL, NULL, NULL);
    r = zway_fc_memory_get_buffer(zway, 0x2345, 16, NULL, NULL, NULL);
}

void test_save(ZWay zway)
{
    ZWError r;
    
    ZWBYTE *data = NULL;
    size_t length = 0;
    r = zway_controller_config_save(zway, &data, &length);
    if (r == NoError)
    {
        printf("Successfully saved! (size = %lu)\n", (unsigned long int) length);
        
        char path[MAX_PATH];
        strcpy(path, "/tmp/config.tgz");
        
        FILE *t = fopen(path, "wb");
        if (t != NULL)
        {
            fwrite(data, 1, length, t);
            fclose(t);
            
            printf("Config written to %s\n", path);
        }
        else
        {
            printf("Config write error: %s\n", strerror(errno));
        }
        
        free(data);
    }
    else
    {
        printf("Error saving configuration! (err = %s)\n", zway_strerror(r));
    }
}

void test_restore(ZWay zway)
{
    char path[MAX_PATH];
    strcpy(path, "/tmp/config.tgz");
    
    printf("Reading config from %s\n", path);
    
    struct stat finfo;
    int tr = stat(path, &finfo);
    if (tr != 0)
    {
        if (errno == ENOENT)
        {
            printf("Config not found. Save one first!\n");
        }
        else
        {
            printf("Config read error: %s\n", strerror(errno));
        }
        return;
    }
    
    size_t length = finfo.st_size;
    ZWBYTE *data = (ZWBYTE*)malloc(length);
    if (data == NULL)
    {
        printf("Failed to alloc %zu bytes!\n", length);
        return;
    }
    
    FILE *t = fopen(path, "rb");
    if (t != NULL)
    {
        fread(data, 1, length, t);
        fclose(t);
    }
    else
    {
        free(data);
        printf("Config read error: %s\n", strerror(errno));
        return;
    }
    
    ZWError r = zway_controller_config_restore(zway, data, length, FALSE);
    if (r == NoError)
    {
        printf("Configuration successfully restored\n");
    }
    else
    {
        printf("Error restoring configuration! (err = %s)\n", zway_strerror(r));
    }
    
    free(data);
}

void dump_data(const ZWay zway, ZDataHolder data)
{
    char *path = zway_data_get_path(zway, data);
    ZWDataType type;
    zway_data_get_type(zway, data, &type);

    ZWBOOL bool_val;
    int int_val;
    float float_val;
    ZWCSTR str_val;
    const ZWBYTE *binary;
    const int *int_arr;
    const float *float_arr;
    const ZWCSTR *str_arr;
    size_t len, i;

    switch (type) 
    {
        case Empty:
            zway_log(zway, Debug, ZSTR("DATA %s = Empty\n"), path);
            break;
        case Boolean:
            zway_data_get_boolean(zway, data, &bool_val);
            if (bool_val)
                zway_log(zway, Debug, ZSTR("DATA %s = True\n"), path);
            else
                zway_log(zway, Debug, ZSTR("DATA %s = False\n"), path);
            break;
        case Integer:
            zway_data_get_integer(zway, data, &int_val);
            zway_log(zway, Debug, ZSTR("DATA %s = %d (0x%08x)\n"), path, int_val, int_val);
            break;
        case Float:
            zway_data_get_float(zway, data, &float_val);
            zway_log(zway, Debug, ZSTR("DATA %s = %f\n"), path, float_val);
            break;
        case String:
            zway_data_get_string(zway, data, &str_val);
            zway_log(zway, Debug, ZSTR("DATA %s = \"%s\"\n"), path, str_val);
            break;
        case Binary:
            zway_data_get_binary(zway, data, &binary, &len);
            zway_log(zway, Debug, ZSTR("DATA %s = byte[%d]\n"), path, len);
            zway_dump(zway, Debug, ZSTR("  "), len, binary);
            break;
        case ArrayOfInteger:
            zway_data_get_integer_array(zway, data, &int_arr, &len);
            zway_log(zway, Debug, ZSTR("DATA %s = int[%d]\n"), path, len);
            for (i = 0; i < len; i++)
                zway_log(zway, Debug, ZSTR("  [%02d] %d\n"), i, int_arr[i]);
            break;
        case ArrayOfFloat:
            zway_data_get_float_array(zway, data, &float_arr, &len);
            zway_log(zway, Debug, ZSTR("DATA %s = float[%d]\n"), path, len);
            for (i = 0; i < len; i++)
                zway_log(zway, Debug, ZSTR("  [%02d] %f\n"), i, float_arr[i]);
            break;
        case ArrayOfString:
            zway_data_get_string_array(zway, data, &str_arr, &len);
            zway_log(zway, Debug, ZSTR("DATA %s = string[%d]\n"), path, len);
            for (i = 0; i < len; i++)
                zway_log(zway, Debug, ZSTR("  [%02d] \"%s\"\n"), i, str_arr[i]);
            break;
    }
    free(path);
    
    ZDataIterator child = zway_data_first_child(zway, data);
    while (child != NULL)
    {
        path = zway_data_get_path(zway, child->data);
        zway_log(zway, Debug, ZSTR("CHILD %s\n"), path);
        free(path);
        child = zway_data_next_child(child);
    }
}

void print_basic_holder(const ZWay zway, ZWDataChangeType type, ZDataHolder data)
{
    int int_val;
    
    zway_data_get_integer(zway, data, &int_val);
    zway_log(zway, Debug, ZSTR("Basic set value = %i\n"), int_val); 
}

void print_D_I_CC_event(const ZWay zway, ZWDeviceChangeType type, ZWBYTE node_id, ZWBYTE instance_id, ZWBYTE command_id, void *arg)
{
    switch (type)
    {
        case  DeviceAdded:
            zway_log(zway, Information, ZSTR("New device added: %i\n"), node_id);
            break;
        
        case DeviceRemoved:
            zway_log(zway, Information, ZSTR("Device removed: %i\n"), node_id);
            break;
        
        case InstanceAdded:
            zway_log(zway, Information, ZSTR("New instance added to device %i: %i\n"), node_id, instance_id);
            break;
        
        case InstanceRemoved:
            zway_log(zway, Information, ZSTR("Instance removed from device %i: %i\n"), node_id, instance_id);
            break;
        
        case CommandAdded:
            zway_log(zway, Information, ZSTR("New Command Class added to device %i:%i: %i\n"), node_id, instance_id, command_id);
            break;
        
        case CommandRemoved:        
            zway_log(zway, Information, ZSTR("Command Class removed from device %i:%i: %i\n"), node_id, instance_id, command_id);
            break;
    }        
}

void print_help(void) {
    printf("=== Test commands ===\n"
           "d r <path>\n"
           "d d <dev> <path>\n"
           "d i <dev> <inst> <path>\n"
           "d c <dev> <inst> <ccId> <path>\n"
           "m Memory test\n"
           "s <dev> <inst> <ccId> (s=set|g=get) <val>\n"
           "a Start AddNodeToNetwork\n"
           "A Stop AddNodeToNetwork\n"
           "e Start RemoveNodeFromNetwork\n"
           "E Stop RemoveNodeFromNetwork\n"
           "S save configuration to tar file\n"
           "R restore configuration from tar file\n"
           "W restore stick from config\n"
           "x exit\n");
}

int do_work(ZWay zway)
{
    print_help();

    char cmd, cc_cmd, holder_root;
    ZWBYTE dev, inst, cc, cc_val, nconv;
    char data_path[256];
    char cmd_buffer[256];
    
    ZWBOOL was_idle = FALSE;
    
    ZWBOOL basic_level_attached = FALSE;
    
    int skip = 0;
    int running = TRUE;    
    while (running)
    {
        if (!zway_is_running(zway))
        {
            running = FALSE;
            break;
        }
        
        if (!zway_is_idle(zway)) 
        {
            sleep_ms(10);
            continue;
        }
        skip = 0;

        if (!basic_level_attached)
        {
            ZDataHolder basic_level_holder;
            zway_data_acquire_lock(zway);
            basic_level_holder = zway_find_device_instance_cc_data(zway, 8, 0, 0x20, "mylevel");
            if (basic_level_holder)
            {
                basic_level_attached = (zway_data_add_callback(zway, basic_level_holder, (ZDataChangeCallback) print_basic_holder, FALSE, NULL) == NoError);
                if (basic_level_attached)
                    zway_log(zway, Debug, ZSTR("Basic.data.mylevel holder handler attached to device 8 instance 0\n"));
            }
            zway_data_release_lock(zway);
        }        
                
        printf("> ");
        fgets(cmd_buffer, 255, stdin);
        was_idle = FALSE;        
        
        nconv = sscanf(cmd_buffer, "%c %*s", &cmd);
        if (nconv > 0)
        {
            switch (cmd)
            {
                case 'h':
                    print_help();
                    break;
                
                case 'd':
                    nconv = sscanf(cmd_buffer, "%c %c", &cmd, &holder_root);
                    if (nconv > 1)
                    {
                        switch(holder_root)
                        {
                            case 'r':
                                nconv = sscanf(cmd_buffer, "%c %c %s", &cmd, &holder_root, data_path);
                                if (nconv >= 2)
                                {
                                    if (nconv == 2)
                                    {
                                        data_path[0] = '.';
                                        data_path[1] = '\0';
                                    }
                                    zway_data_acquire_lock(zway);
                                    dump_data(zway, zway_find_controller_data(zway, data_path));
                                    zway_data_release_lock(zway);
                                }
                                break;
                                
                            case 'd':
                                nconv = sscanf(cmd_buffer, "%c %c %hhd %s", &cmd, &holder_root, &dev, data_path);
                                if (nconv >= 3)
                                {
                                    if (nconv == 3)
                                    {
                                        data_path[0] = '.';
                                        data_path[1] = '\0';
                                    }
                                    zway_data_acquire_lock(zway);
                                    dump_data(zway, zway_find_device_data(zway, dev, data_path));
                                    zway_data_release_lock(zway);
                                }
                                break;
                            case 'i':
                                nconv = sscanf(cmd_buffer, "%c %c %hhd %hhd %s", &cmd, &holder_root, &dev, &inst, data_path);
                                if (nconv >= 4)
                                {
                                    if (nconv == 4)
                                    {
                                        data_path[0] = '.';
                                        data_path[1] = '\0';
                                    }
                                    zway_data_acquire_lock(zway);
                                    dump_data(zway, zway_find_device_instance_data(zway, dev, inst, data_path));
                                    zway_data_release_lock(zway);
                                }
                                break;
                            case 'c':
                                nconv = sscanf(cmd_buffer, "%c %c %hhd %hhd %hhd %s", &cmd, &holder_root, &dev, &inst, &cc, data_path);
                                if (nconv >= 5)
                                {
                                    if (nconv == 5)
                                    {
                                        data_path[0] = '.';
                                        data_path[1] = '\0';
                                    }
                                    zway_data_acquire_lock(zway);
                                    dump_data(zway, zway_find_device_instance_cc_data(zway, dev, inst, cc, data_path));
                                    zway_data_release_lock(zway);
                                }
                                break;
                        }
                    }
                    break;

                case 's':
                    nconv = sscanf(cmd_buffer, "%c %hhd %hhd %hhd %c %hhd", &cmd, &dev, &inst, &cc, &cc_cmd, &cc_val);
                    
                    if (nconv == 6 && cmd == 's' && cc == 0x20 && cc_cmd == 's')
                    {
                        zway_data_acquire_lock(zway);
                        zway_cc_basic_set(zway, dev, inst, cc_val, NULL, NULL, NULL);
                        zway_data_release_lock(zway);
                    }
                    else if (nconv == 5 && cmd == 's' && cc == 0x20 && cc_cmd == 'g')
                    {
                        zway_data_acquire_lock(zway);
                        zway_cc_basic_get(zway, dev, inst, NULL, NULL, NULL);
                        zway_data_release_lock(zway);
                    }
                    break;
                
                case 'n':
                    nconv = sscanf(cmd_buffer, "%c %hhd", &cmd, &dev);
                    
                    if (nconv == 2)
                        zway_fc_request_node_information(zway, dev, NULL, NULL, NULL);
                    break;
                
                case 'm':
                    test_memory(zway);
                    break;

                case 'a':
                    zway_fc_add_node_to_network(zway, TRUE, TRUE, NULL, NULL, NULL);
                    break;
                    
                case 'e':
                    zway_fc_remove_node_from_network(zway, TRUE, TRUE, NULL, NULL, NULL);
                    break;
                    
                case 'x':
                    running = FALSE;
                    break;

                case 'S':
                    test_save(zway);
                    break;
                    
                case 'R':
                    test_restore(zway);
                    break;
                    
                case 'l':
                    nconv = sscanf(cmd_buffer, "%c %hhd %hhd", &cmd, &dev, &inst);
                    switch(nconv)
                    {
                        case 1:
                            {
                                ZWDevicesList list = zway_devices_list(zway);
                                if (list != NULL) {
                                    int i = 0;
                                    printf("Devices list: ");
                                    while (list[i]) {
                                        printf("%i ", list[i]);
                                        i++;
                                    }
                                    zway_devices_list_free(list);
                                    printf("\n");
                                } else
                                    printf("Error happened requesting devices list\n");
                            }
                            break;

                        case 2:
                            {
                                ZWInstancesList list = zway_instances_list(zway, dev);
                                if (list != NULL) {
                                    int i = 0;
                                    printf("Instances list for device %i: ", dev);
                                    while (list[i]) {
                                        printf("%i ", list[i]);
                                        i++;
                                    }
                                    zway_instances_list_free(list);
                                    printf("\n");
                                } else
                                    printf("Error happened requesting instances list\n");
                            }
                            break;

                        case 3:
                            {
                                ZWCommandClassesList list = zway_command_classes_list(zway, dev, inst);
                                if (list != NULL) {
                                    int i = 0;
                                    printf("Command Classes list for device %i instance %i: ", dev, inst);
                                    while (list[i]) {
                                        printf("%02x ", list[i]);
                                        i++;
                                    }
                                    zway_command_classes_list_free(list);
                                    printf("\n");
                                } else
                                    printf("Error happened requesting command classes list\n");
                            }
                            break;
                    }
                    break;
            }
        }
    }

    return 0;
}

void print_zway_terminated(ZWay zway)
{
    zway_log(zway, Information, ZSTR("Z-Way terminated\n")); 
}


int main (int argc, const char * argv[])
{
    ZWay zway = NULL;
    #ifdef _WINDOWS
    ZWError r = zway_init(&zway, ZSTR("COM3"), NULL, NULL, NULL, NULL, Debug);
    #endif
    #ifdef __MACH__
    ZWError r = zway_init(&zway, ZSTR("/dev/cu.SLAB_USBtoUART"), NULL, NULL, NULL, NULL, Debug);
    #endif
    #ifdef __linux__
    ZWError r = zway_init(&zway, ZSTR("/dev/ttyUSB0"), NULL, NULL, NULL, NULL, Debug);
    #endif
    if (r != NoError)
    {
        zway_log_error(zway, Critical, "Failed to init ZWay", r);
        return -1;
    }

    zway_device_add_callback(zway, DeviceAdded | DeviceRemoved | InstanceAdded | InstanceRemoved | CommandAdded | CommandRemoved, print_D_I_CC_event, NULL);
    
    r = zway_start(zway, print_zway_terminated);
    if (r != NoError)
    {
        zway_log_error(zway, Critical, "Failed to start ZWay", r);
        return -1;
    }

    r = zway_discover(zway);
    if (r != NoError)
    {
        zway_log_error(zway, Critical, "Failed to negotiate with Z-Wave stick", r);
        return -1;
    }

    // Application code
    int code = do_work(zway);

    r = zway_stop(zway);
    
    zway_terminate(&zway);
    
    return code;
}

