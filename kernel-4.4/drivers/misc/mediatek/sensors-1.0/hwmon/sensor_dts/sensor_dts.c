/*
* Copyright (C) 2011-2014 MediaTek Inc.
*
* This program is free software: you can redistribute it and/or modify it under the terms of the
* GNU General Public License version 2 as published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program.
* If not, see <http://www.gnu.org/licenses/>.
*/

#include <linux/of.h>
#include <linux/of_irq.h>
#include <cust_alsps.h>
#include <cust_acc.h>
#include <cust_gyro.h>
#include <cust_mag.h>
#include <cust_baro.h>
#include <cust_hmdy.h>
#include <cust_accelgyro.h>

#define SENSOR_TAG				  "[Sensor dts] "
#define SENSOR_PR_ERR(fmt, args...)	pr_err(SENSOR_TAG fmt, ##args)
#define SENSOR_LOG(fmt, args...)	pr_debug(SENSOR_TAG fmt, ##args)

int get_accel_dts_func(struct device_node *node, struct acc_hw *hw)
{
	int i, ret;
	u32 i2c_num[] = {0};
	u32 i2c_addr[G_CUST_I2C_ADDR_NUM] = {0};
	u32 direction[] = {0};
	u32 power_id[] = {0};
	u32 power_vol[] = {0};
	u32 firlen[] = {0};
	u32 is_batch_supported[] = {0};
	u32 calib_enable[] = {0};
	u32 auto_calib_enable[] = {0};

	SENSOR_LOG("Device Tree get accel info!\n");
	if (node) {
		ret = of_property_read_u32_array(node, "i2c_num", i2c_num, ARRAY_SIZE(i2c_num));
		if (ret == 0)
			hw->i2c_num	=	i2c_num[0];

		ret = of_property_read_u32_array(node, "i2c_addr", i2c_addr, ARRAY_SIZE(i2c_addr));
		if (ret == 0) {
			for (i = 0; i < G_CUST_I2C_ADDR_NUM; i++)
				hw->i2c_addr[i] = i2c_addr[i];
		}

		ret = of_property_read_u32_array(node, "direction", direction, ARRAY_SIZE(direction));
		if (ret == 0)
			hw->direction = direction[0];

		ret = of_property_read_u32_array(node, "power_id", power_id, ARRAY_SIZE(power_id));
		if (ret == 0) {
			if (power_id[0] == 0xffff)
				hw->power_id = -1;
			else
				hw->power_id	=	power_id[0];
		}

		ret = of_property_read_u32_array(node, "power_vol", power_vol, ARRAY_SIZE(power_vol));
		if (ret == 0)
			hw->power_vol	=	power_vol[0];

		ret = of_property_read_u32_array(node, "firlen", firlen, ARRAY_SIZE(firlen));
		if (ret == 0)
			hw->firlen	=	firlen[0];

		ret = of_property_read_u32_array(node, "is_batch_supported",
			is_batch_supported, ARRAY_SIZE(is_batch_supported));
		if (ret == 0)
			hw->is_batch_supported		 = is_batch_supported[0];
		ret = of_property_read_u32_array(node , "calib_enable", calib_enable, ARRAY_SIZE(calib_enable));
                if (ret == 0)
                        hw->calib_enable = calib_enable[0];
		ret = of_property_read_u32_array(node , "auto_calib_enable", auto_calib_enable, ARRAY_SIZE(auto_calib_enable));
                if (ret == 0)
                        hw->auto_calib_enable = auto_calib_enable[0];
	} else {
		SENSOR_PR_ERR("Device Tree: can not find accel node!. Go to use old cust info\n");
		return -1;
	}

	return 0;
}


int get_alsps_dts_func(struct device_node *node, struct alsps_hw *hw)
{
	int i, ret;
	u32 i2c_num[] = {0};
	u32 i2c_addr[C_CUST_I2C_ADDR_NUM] = {0};
	u32 power_id[] = {0};
	u32 power_vol[] = {0};
	u32 polling_mode_ps[] = {0};
	u32 polling_mode_als[] = {0};
	u32 als_level[TP_COUNT*TEMP_COUNT*C_CUST_ALS_LEVEL] = {0};  //modify by hzb
	u32 als_value[C_CUST_ALS_LEVEL] = {0};
	u32 ps_threshold_high[] = {0};
	u32 ps_threshold_low[] = {0};
	u32 is_batch_supported_ps[] = {0};
	u32 is_batch_supported_als[] = {0};
	u32 get_val[] = {0};  //add by hzb

	SENSOR_LOG("Device Tree get alsps info!\n");
	if (node) {
		ret = of_property_read_u32_array(node, "i2c_num", i2c_num, ARRAY_SIZE(i2c_num));
	if (ret == 0)
		hw->i2c_num	=	i2c_num[0];

	ret = of_property_read_u32_array(node, "i2c_addr", i2c_addr, ARRAY_SIZE(i2c_addr));
	if (ret == 0) {
		for (i = 0; i < C_CUST_I2C_ADDR_NUM; i++)
			hw->i2c_addr[i]   = i2c_addr[i];
	}

	ret = of_property_read_u32_array(node, "power_id", power_id, ARRAY_SIZE(power_id));
	if (ret == 0) {
		if (power_id[0] == 0xffff)
			hw->power_id = -1;
		else
			hw->power_id	=	power_id[0];
	}

	ret = of_property_read_u32_array(node, "power_vol", power_vol, ARRAY_SIZE(power_vol));
	if (ret == 0)
		hw->power_vol	=	power_vol[0];

	ret = of_property_read_u32_array(node, "als_level", als_level, ARRAY_SIZE(als_level));
	if (ret == 0) {
			memcpy(hw->als_level,als_level,sizeof(als_level));
#if 0
		for (i = 0; i < ARRAY_SIZE(als_level); i++)
			hw->als_level[i]		 = als_level[i];
#endif
	}

	ret = of_property_read_u32_array(node , "state_val", get_val, ARRAY_SIZE(get_val));
	if (ret == 0)
		hw->state_val		 = get_val[0];
	ret = of_property_read_u32_array(node , "psctrl_val", get_val, ARRAY_SIZE(get_val));
	if (ret == 0)
		hw->psctrl_val		 = get_val[0];
	ret = of_property_read_u32_array(node , "alsctrl_val", get_val, ARRAY_SIZE(get_val));
	if (ret == 0)
		hw->alsctrl_val		 = get_val[0];
	ret = of_property_read_u32_array(node , "ledctrl_val", get_val, ARRAY_SIZE(get_val));
	if (ret == 0)
		hw->ledctrl_val		 = get_val[0];
	ret = of_property_read_u32_array(node , "wait_val", get_val, ARRAY_SIZE(get_val));
	if (ret == 0)
		hw->wait_val		 = get_val[0];
	ret = of_property_read_u32_array(node, "als_value", als_value, ARRAY_SIZE(als_value));
	if (ret == 0) {
		for (i = 0; i < ARRAY_SIZE(als_value); i++)
			hw->als_value[i]		 = als_value[i];
	}

	ret = of_property_read_u32_array(node, "polling_mode_ps", polling_mode_ps, ARRAY_SIZE(polling_mode_ps));
	if (ret == 0)
		hw->polling_mode_ps		 = polling_mode_ps[0];

	ret = of_property_read_u32_array(node, "polling_mode_als", polling_mode_als, ARRAY_SIZE(polling_mode_als));
	if (ret == 0)
		hw->polling_mode_als		 = polling_mode_als[0];

	ret = of_property_read_u32_array(node, "ps_threshold_high", ps_threshold_high, ARRAY_SIZE(ps_threshold_high));
	if (ret == 0)
		hw->ps_threshold_high		 = ps_threshold_high[0];

	ret = of_property_read_u32_array(node, "ps_threshold_low", ps_threshold_low, ARRAY_SIZE(ps_threshold_low));
	if (ret == 0)
		hw->ps_threshold_low		 = ps_threshold_low[0];

	ret = of_property_read_u32_array(node, "is_batch_supported_ps", is_batch_supported_ps,
		ARRAY_SIZE(is_batch_supported_ps));
	if (ret == 0)
		hw->is_batch_supported_ps		 = is_batch_supported_ps[0];

	ret = of_property_read_u32_array(node, "is_batch_supported_als", is_batch_supported_als,
		ARRAY_SIZE(is_batch_supported_als));
	if (ret == 0)
		hw->is_batch_supported_als		 = is_batch_supported_als[0];
	} else {
		SENSOR_PR_ERR("Device Tree: can not find alsps node!. Go to use old cust info\n");
		return -1;
	}
	return 0;
}

int get_mag_dts_func(struct device_node *node, struct mag_hw *hw)
{
	int i, ret;
	u32 i2c_num[] = {0};
	u32 i2c_addr[M_CUST_I2C_ADDR_NUM] = {0};
	u32 direction[] = {0};
	u32 power_id[] = {0};
	u32 power_vol[] = {0};
	u32 is_batch_supported[] = {0};

	SENSOR_LOG("Device Tree get mag info!\n");
	if (node) {
		ret = of_property_read_u32_array(node, "i2c_num", i2c_num, ARRAY_SIZE(i2c_num));
		if (ret == 0)
			hw->i2c_num	=	i2c_num[0];

		ret = of_property_read_u32_array(node, "i2c_addr", i2c_addr, ARRAY_SIZE(i2c_addr));
		if (ret == 0) {
			for (i = 0; i < M_CUST_I2C_ADDR_NUM; i++)
				hw->i2c_addr[i]   = i2c_addr[i];
		}

		ret = of_property_read_u32_array(node, "direction", direction, ARRAY_SIZE(direction));
		if (ret == 0)
			hw->direction = direction[0];

		ret = of_property_read_u32_array(node, "power_id", power_id, ARRAY_SIZE(power_id));
		if (ret == 0) {
			if (power_id[0] == 0xffff)
				hw->power_id = -1;
			else
				hw->power_id	=	 power_id[0];
		}

		ret = of_property_read_u32_array(node, "power_vol", power_vol, ARRAY_SIZE(power_vol));
		if (ret == 0)
			hw->power_vol	 =	  power_vol[0];

		ret = of_property_read_u32_array(node, "is_batch_supported", is_batch_supported,
			ARRAY_SIZE(is_batch_supported));
		if (ret == 0)
			hw->is_batch_supported		   = is_batch_supported[0];
	} else {
		SENSOR_PR_ERR("Device Tree: can not find mag node!. Go to use old cust info\n");
		return -1;
	}
	return 0;
}

int get_gyro_dts_func(struct device_node *node, struct gyro_hw *hw)
{
	int i, ret;
	u32 i2c_num[] = {0};
	u32 i2c_addr[C_CUST_I2C_ADDR_NUM] = {0};
	u32 direction[] = {0};
	u32 power_id[] = {0};
	u32 power_vol[] = {0};
	u32 firlen[] = {0};
	u32 is_batch_supported[] = {0};

	SENSOR_LOG("Device Tree get gyro info!\n");
	if (node) {
		ret = of_property_read_u32_array(node, "i2c_num", i2c_num, ARRAY_SIZE(i2c_num));
		if (ret == 0)
			hw->i2c_num	=	i2c_num[0];

		ret = of_property_read_u32_array(node, "i2c_addr", i2c_addr, ARRAY_SIZE(i2c_addr));
		if (ret == 0) {
			for (i = 0; i < GYRO_CUST_I2C_ADDR_NUM; i++)
				hw->i2c_addr[i] = i2c_addr[i];
		}

		ret = of_property_read_u32_array(node, "direction", direction, ARRAY_SIZE(direction));
		if (ret == 0)
			hw->direction = direction[0];

		ret = of_property_read_u32_array(node, "power_id", power_id, ARRAY_SIZE(power_id));
		if (ret == 0) {
			if (power_id[0] == 0xffff)
				hw->power_id = -1;
			else
				hw->power_id	=	power_id[0];
		}

		ret = of_property_read_u32_array(node, "power_vol", power_vol, ARRAY_SIZE(power_vol));
		if (ret == 0)
			hw->power_vol	=	power_vol[0];

		ret = of_property_read_u32_array(node, "firlen", firlen, ARRAY_SIZE(firlen));
		if (ret == 0)
			hw->firlen	=	firlen[0];

		ret = of_property_read_u32_array(node, "is_batch_supported", is_batch_supported,
			ARRAY_SIZE(is_batch_supported));
		if (ret == 0)
			hw->is_batch_supported		 = is_batch_supported[0];
	} else {
		SENSOR_PR_ERR("Device Tree: can not find gyro node!. Go to use old cust info\n");
		return -1;
	}
	return 0;
}

int get_baro_dts_func(struct device_node *node, struct baro_hw *hw)
{
	int i, ret;
	u32 i2c_num[] = {0};
	u32 i2c_addr[C_CUST_I2C_ADDR_NUM] = {0};
	u32 direction[] = {0};
	u32 power_id[] = {0};
	u32 power_vol[] = {0};
	u32 firlen[] = {0};
	u32 is_batch_supported[] = {0};

	SENSOR_LOG("Device Tree get gyro info!\n");

	if (node) {
		ret = of_property_read_u32_array(node, "i2c_num", i2c_num, ARRAY_SIZE(i2c_num));
		if (ret == 0)
			hw->i2c_num	=	i2c_num[0];

		ret = of_property_read_u32_array(node, "i2c_addr", i2c_addr, ARRAY_SIZE(i2c_addr));
		if (ret == 0) {
			for (i = 0; i < GYRO_CUST_I2C_ADDR_NUM; i++)
				hw->i2c_addr[i] = i2c_addr[i];
		}

		ret = of_property_read_u32_array(node, "direction", direction, ARRAY_SIZE(direction));
		if (ret == 0)
			hw->direction = direction[0];

		ret = of_property_read_u32_array(node, "power_id", power_id, ARRAY_SIZE(power_id));
		if (ret == 0) {
			if (power_id[0] == 0xffff)
				hw->power_id = -1;
			else
				hw->power_id	=	power_id[0];
		}

		ret = of_property_read_u32_array(node, "power_vol", power_vol, ARRAY_SIZE(power_vol));
		if (ret == 0)
			hw->power_vol	=	power_vol[0];

		ret = of_property_read_u32_array(node, "firlen", firlen, ARRAY_SIZE(firlen));
		if (ret == 0)
			hw->firlen	=	firlen[0];

		ret = of_property_read_u32_array(node, "is_batch_supported", is_batch_supported,
			ARRAY_SIZE(is_batch_supported));
		if (ret == 0)
			hw->is_batch_supported		 = is_batch_supported[0];
	} else {
		SENSOR_PR_ERR("Device Tree: can not find gyro node!. Go to use old cust info\n");
		return -1;
	}
	return 0;
}

int get_hmdy_dts_func(const char *name, struct hmdy_hw *hw)
{
	int i, ret;
	u32 i2c_num[] = {0};
	u32 i2c_addr[C_CUST_I2C_ADDR_NUM] = {0};
	u32 direction[] = {0};
	u32 power_id[] = {0};
	u32 power_vol[] = {0};
	u32 firlen[] = {0};
	u32 is_batch_supported[] = {0};
	struct device_node *node = NULL;

	SENSOR_LOG("Device Tree get gyro info!\n");
	if (name == NULL)
		return -1;

	node = of_find_compatible_node(NULL, NULL, name);
	if (node) {
		ret = of_property_read_u32_array(node, "i2c_num", i2c_num, ARRAY_SIZE(i2c_num));
		if (ret == 0)
			hw->i2c_num	=	i2c_num[0];

		ret = of_property_read_u32_array(node, "i2c_addr", i2c_addr, ARRAY_SIZE(i2c_addr));
		if (ret == 0) {
			for (i = 0; i < GYRO_CUST_I2C_ADDR_NUM; i++)
				hw->i2c_addr[i] = i2c_addr[i];
		}

		ret = of_property_read_u32_array(node, "direction", direction, ARRAY_SIZE(direction));
		if (ret == 0)
			hw->direction = direction[0];

		ret = of_property_read_u32_array(node, "power_id", power_id, ARRAY_SIZE(power_id));
		if (ret == 0) {
			if (power_id[0] == 0xffff)
				hw->power_id = -1;
			else
				hw->power_id	=	power_id[0];
		}

		ret = of_property_read_u32_array(node, "power_vol", power_vol, ARRAY_SIZE(power_vol));
		if (ret == 0)
			hw->power_vol	=	power_vol[0];

		ret = of_property_read_u32_array(node, "firlen", firlen, ARRAY_SIZE(firlen));
		if (ret == 0)
			hw->firlen	=	firlen[0];

		ret = of_property_read_u32_array(node, "is_batch_supported", is_batch_supported,
			ARRAY_SIZE(is_batch_supported));
		if (ret == 0)
			hw->is_batch_supported		 = is_batch_supported[0];
	} else {
		SENSOR_PR_ERR("Device Tree: can not find gyro node!. Go to use old cust info\n");
		return -1;
	}
	return 0;
}

int get_accelgyro_dts_func(struct device_node *node, struct accelgyro_hw *hw)
{
	int ret;

	u32 direction[] = {0};
	u32 accel_firlen[] = {0};
	u32 gyro_firlen[] = {0};
	u32 accel_is_batch_supported[] = {0};
	u32 gyro_is_batch_supported[] = {0};

	SENSOR_LOG("Device Tree get accel info!\n");
	if (node) {
		ret = of_property_read_u32_array(node, "direction", direction, ARRAY_SIZE(direction));
		if (ret == 0)
			hw->direction = direction[0];

		ret = of_property_read_u32_array(node, "accel_firlen", accel_firlen, ARRAY_SIZE(accel_firlen));
		if (ret == 0)
			hw->accel_firlen	=	accel_firlen[0];

		ret = of_property_read_u32_array(node, "accel_is_batch_supported",
			accel_is_batch_supported, ARRAY_SIZE(accel_is_batch_supported));
		if (ret == 0)
			hw->accel_is_batch_supported		 = accel_is_batch_supported[0];


		ret = of_property_read_u32_array(node, "gyro_firlen", gyro_firlen, ARRAY_SIZE(gyro_firlen));
		if (ret == 0)
			hw->gyro_firlen	=	gyro_firlen[0];

		ret = of_property_read_u32_array(node, "gyro_is_batch_supported", gyro_is_batch_supported,
			ARRAY_SIZE(gyro_is_batch_supported));
		if (ret == 0)
			hw->gyro_is_batch_supported		 = gyro_is_batch_supported[0];
	} else {
		SENSOR_PR_ERR("Device Tree: can not find accel node!. Go to use old cust info\n");
		return -1;
	}

	return 0;
}
