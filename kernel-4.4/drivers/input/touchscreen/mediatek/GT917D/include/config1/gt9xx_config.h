#ifndef _GT9XX_CONFIG_H_
#define _GT9XX_CONFIG_H_

/* ***************************PART2:TODO define********************************** */
/* STEP_1(REQUIRED):Change config table. */
/*TODO: puts the config info corresponded to your TP here, the following is just
a sample config, send this config should cause the chip cannot work normally*/
#define CTP_CFG_GROUP0 {\
0x00,0xD0,0x02,0xA0,0x05,0x0A,0xFD,0x30,0x11,0x8F,0x2F,0x0F,\
0x5F,0x46,0x03,0x05,0x00,0x00,0x60,0x62,0x01,0x12,0x05,0x16,\
0x19,0x1F,0x14,0xE7,0x26,0xCD,0x48,0x4A,0xB8,0x08,0x7C,0x06,\
0x00,0xC3,0x33,0x91,0x00,0x00,0x00,0x00,0x00,0x02,0xC0,0x08,\
0x0C,0x00,0x1A,0x28,0x50,0x8A,0x90,0x64,0x19,0x28,0x00,0x04,\
0x94,0x2A,0x00,0x84,0x31,0x00,0x78,0x38,0x00,0x6E,0x41,0x00,\
0x66,0x4A,0x00,0x66,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,\
0x00,0x00,0x0F,0x32,0x00,0x03,0x00,0x00,0x32,0x06,0x55,0x00,\
0x00,0x00,0x00,0x41,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,\
0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,\
0x14,0x15,0x16,0x17,0x18,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x02,\
0x04,0x05,0x06,0x08,0x0A,0x1E,0x1F,0x20,0x22,0x24,0x2A,0xFF,\
0xFF,0xFF,0xFF,0x8C,0x31,0x3C,0x3C,0x00,0x8C,0x70,0x12,0x00,\
0x32,0x4B,0x5A,0x19,0x1E,0x3C,0x00,0x00,0x06,0x66,0x66,0x0F,\
0x10,0x88,0x85,0xC5,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x32,0x28,0x00,0x00,0x0F,0x14,0x00,0x37,\
0x53,0x00,0x78,0x78,0x95,0x00,0x55,0x00,0x3C,0x42,0x00,0x00,\
0x5A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x01\
    }

#define GTP_CFG_GROUP0_CHARGER {\
}

/* TODO puts your group2 config info here,if need. */
#define CTP_CFG_GROUP1 {\
}

/* TODO puts your group2 config info here,if need. */
#define GTP_CFG_GROUP1_CHARGER {\
}

/* TODO puts your group3 config info here,if need. */
#define CTP_CFG_GROUP2 {\
}

/* TODO puts your group3 config info here,if need. */
#define GTP_CFG_GROUP2_CHARGER {\
}

/* TODO: define your config for Sensor_ID == 3 here, if needed */
#define CTP_CFG_GROUP3 {\
}

/* TODO puts your group3 config info here,if need. */
#define GTP_CFG_GROUP3_CHARGER {\
}

/* TODO: define your config for Sensor_ID == 4 here, if needed */
#define CTP_CFG_GROUP4 {\
}

/* TODO puts your group4 config info here,if need. */
#define GTP_CFG_GROUP4_CHARGER {\
}

/* TODO: define your config for Sensor_ID == 5 here, if needed */
#define CTP_CFG_GROUP5 {\
}

/* TODO puts your group5 config info here,if need. */
#define GTP_CFG_GROUP5_CHARGER {\
}


#endif /* _GT1X_CONFIG_H_ */
