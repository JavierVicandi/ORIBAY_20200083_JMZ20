//IIO driver for the light sensor ISL76683.
//
// Copyright (c) 2017 Christoph Fritz <chf.fritz@googlemail.com>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.
//
// Datasheet:
//  http://www.intersil.com/content/dam/Intersil/documents/isl7/isl76683.pdf

#define ISL76683_REG_CMD		0x00
#define ISL76683_REG_CTRL		0x01
#define ISL76683_REG_THR_HI		0x02
#define ISL76683_REG_THR_LO		0x03
#define ISL76683_REG_SENSOR_LSB		0x04
#define ISL76683_REG_SENSOR_MSB		0x05
#define ISL76683_REG_CLEAR_INT		0x40
#define ISL76683_REGMAP_MAX		0x40

#define ISL76683_CMD_ENABLE		BIT(7)
#define ISL76683_CMD_PWRDWN		BIT(6)
#define ISL76683_WIDTH_MASK		0x3
#define ISL76683_PHOTOD_SHFT		2
#define ISL76683_PHOTOD_MASK		GENMASK(3, ISL76683_PHOTOD_SHFT)
#define ISL76683_INTPERS_MASK		0x3
#define ISL76683_LUXRANGE_SHFT		2
#define ISL76683_LUXRANGE_MASK		(0x3 << ISL76683_LUXRANGE_SHFT)
#define ISL76683_LUXRANGE_STR		"1000 4000 16000 64000"

enum isl76683_dmode {
	ISL76683_DIODE_0 = 0,
	ISL76683_DIODE_IR,
	ISL76683_DIODE_DIFF,
};

> +enum isl76683_lux_range {
> +	ISL76683_LUX_1000 = 0,
> +	ISL76683_LUX_4000,
> +	ISL76683_LUX_16000,
> +	ISL76683_LUX_64000,
> +};
> +
> +static const int isl76683_lux_ranges_available[] = {
> +	1000, 4000, 16000, 64000};
> +
> +#define ISL76683_LUX_RANGE_DEFAULT	ISL76683_LUX_1000
> +#define ISL76683_DIODE_MAX		ISL76683_DIODE_DIFF
> +#define ISL76683_DIODE_DEFAULT		ISL76683_DIODE_0
> +#define ISL76683_WIDTH_DEFAULT		0x0
> +#define ISL76683_RESOLUTION_DEFAULT	16
> +#define ISL76683_EXT_RESISTOR_DEFAULT	100
> +#define ISL76683_KOHM_MIN		1
> +#define ISL76683_KOHM_MAX		1000
> +#define ISL76683_INTPERS_DEFAULT	0x0
> +#define ISL76683_THR_DEFAULT		0x7f
> +
> +struct isl76683_chip {
> +	enum isl76683_lux_range	luxrange;
> +	int			external_resistor;
> +	enum isl76683_dmode	photodiode;
> +	struct i2c_client	*client;
> +	struct regmap		*rmp;
> +	struct completion	irq_complete;
> +	struct iio_trigger	*trig;
> +	bool			trig_enabled;
> +	struct mutex		lock;
> +	s64			time_ns;
> +};
> +
> +static bool isl76683_readable_reg(struct device *dev, unsigned int reg)
> +{
> +	switch (reg) {
> +	case ISL76683_REG_CMD:
> +	case ISL76683_REG_CTRL:
> +	case ISL76683_REG_THR_HI:
> +	case ISL76683_REG_THR_LO:
> +	case ISL76683_REG_SENSOR_LSB:
> +	case ISL76683_REG_SENSOR_MSB:
> +	case ISL76683_REG_CLEAR_INT:
> +		return true;
> +	default:
> +		return false;
> +	}
> +}
> +
> +static bool isl76683_writeable_reg(struct device *dev, unsigned int reg)
> +{
> +	switch (reg) {
> +	case ISL76683_REG_CMD:
> +	case ISL76683_REG_CTRL:
> +	case ISL76683_REG_THR_HI:
> +	case ISL76683_REG_THR_LO:
> +		return true;
> +	default:
> +		return false;
> +	}
> +}
> +
> +static bool isl76683_is_volatile_reg(struct device *dev, unsigned int reg)
> +{
> +	switch (reg) {
> +	case ISL76683_REG_SENSOR_LSB:
> +	case ISL76683_REG_SENSOR_MSB:
> +	case ISL76683_REG_CLEAR_INT:
> +		return true;
> +	default:
> +		return false;
> +	}
> +}
> +
> +static const struct regmap_config isl76683_regmap_config = {
> +	.reg_bits = 8,
> +	.val_bits = 8,
> +	.max_register = ISL76683_REGMAP_MAX,
> +	.readable_reg = isl76683_readable_reg,
> +	.writeable_reg = isl76683_writeable_reg,
> +	.volatile_reg = isl76683_is_volatile_reg,
> +	.cache_type = REGCACHE_RBTREE,
> +};
> +
> +static int isl76683_set_config(struct isl76683_chip *chip)
> +{
> +	int ret;
> +
> +	ret = regmap_update_bits(chip->rmp, ISL76683_REG_CTRL,
> +				ISL76683_LUXRANGE_MASK | ISL76683_INTPERS_MASK,
> +				(chip->luxrange << ISL76683_LUXRANGE_SHFT) |
> +				ISL76683_INTPERS_DEFAULT);
> +	if (ret < 0)
> +		return ret;
> +
> +	ret = regmap_update_bits(chip->rmp, ISL76683_REG_CMD,
> +				ISL76683_PHOTOD_MASK | ISL76683_WIDTH_MASK,
> +				(chip->photodiode << ISL76683_PHOTOD_SHFT) |
> +				ISL76683_WIDTH_DEFAULT);
> +	if (ret < 0)
> +		return ret;
> +
> +	ret = regmap_write(chip->rmp, ISL76683_REG_THR_HI,
> +				ISL76683_THR_DEFAULT);
> +	if (ret < 0)
> +		return ret;
> +
> +	return regmap_write(chip->rmp, ISL76683_REG_THR_LO,
> +				ISL76683_THR_DEFAULT);
> +}
> +
> +static int isl76683_power(struct isl76683_chip *chip, bool on)
> +{
> +	int ret;
> +
> +	ret = regmap_update_bits(chip->rmp, ISL76683_REG_CMD,
> +				ISL76683_CMD_ENABLE | ISL76683_CMD_PWRDWN,
> +				0x0);
> +	if (ret < 0)
> +		return ret;
> +
> +	ret = regmap_update_bits(chip->rmp, ISL76683_REG_CMD,
> +				ISL76683_CMD_ENABLE | ISL76683_CMD_PWRDWN,
> +				on ? ISL76683_CMD_ENABLE : ISL76683_CMD_PWRDWN);
> +	if (ret < 0)
> +		return ret;
> +
> +	return on ? isl76683_set_config(chip) : 0;
> +}
> +
> +static int isl76683_reset(struct isl76683_chip *chip)
> +{
> +	int ret;
> +
> +	ret = isl76683_power(chip, false);
> +	if (ret < 0)
> +		return ret;
> +
> +	return isl76683_power(chip, true);
> +}
> +
> +static int isl76683_get_sensordata(struct isl76683_chip *chip,
> +			     bool is_processed, int *val)
> +{
> +	unsigned int sensor_data, range, fsr;
> +	__le16 sensor_raw;
> +	int ret;
> +
> +	ret = regmap_bulk_read(chip->rmp, ISL76683_REG_SENSOR_LSB,
> +				&sensor_raw, sizeof(sensor_raw));
> +	if (ret)
> +		return ret;
> +
> +	sensor_data = le16_to_cpu(sensor_raw);
> +
> +	if (!is_processed) {
> +		*val = sensor_data;
> +		return 0;
> +	}
> +
> +	/* range values taken from datasheet (table 9) */
> +	if (chip->luxrange == ISL76683_LUX_1000)
> +		range = 973;
> +	else if (chip->luxrange == ISL76683_LUX_4000)
> +		range = 3892;
> +	else if (chip->luxrange == ISL76683_LUX_16000)
> +		range = 15568;
> +	else if (chip->luxrange == ISL76683_LUX_64000)
> +		range = 62272;
> +	else
> +		return -EINVAL;
> +
> +	/* equations from datasheet (EQ.3 and EQ.4) */
> +	fsr = (100 * range) / chip->external_resistor;
> +	*val = (fsr * sensor_data) / (1 << ISL76683_RESOLUTION_DEFAULT);

Why is it necessary to do this processing in kernel?

> +
> +	return 0;
> +}
> +
> +static irqreturn_t isl76683_interrupt_handler(int irq, void *private)
> +{
> +	struct iio_dev *indio_dev = private;
> +	struct isl76683_chip *chip = iio_priv(indio_dev);
> +
> +	chip->time_ns = iio_get_time_ns(indio_dev);
> +
> +	if (chip->trig_enabled)
> +		iio_trigger_poll(chip->trig);
> +
> +	if (!completion_done(&chip->irq_complete))
> +		complete(&chip->irq_complete);
> +
> +	return IRQ_HANDLED;
> +}
> +
> +static inline int isl76683_start_measurement(struct isl76683_chip *chip)
> +{
> +	int dummy;
> +
> +	/* dummy read is clearing irq and triggers new measurement */
> +	return regmap_read(chip->rmp, ISL76683_REG_CLEAR_INT, &dummy);
> +}
> +
> +static int isl76683_singleshot_conversion(struct isl76683_chip *chip,
> +					  const struct iio_chan_spec *chan,
> +					  bool is_processed, int *val)
> +{
> +	long timeout;
> +	int ret;
> +
> +	switch (chan->channel2) {
> +	case IIO_MOD_LIGHT_BOTH:
> +		chip->photodiode = ISL76683_DIODE_DIFF;
> +		break;
> +	case IIO_MOD_LIGHT_IR:
> +		chip->photodiode = ISL76683_DIODE_IR;
> +		break;
> +	default:
> +		chip->photodiode = ISL76683_DIODE_0;
> +	}
> +
> +	ret = isl76683_set_config(chip);
> +	if (ret)
> +		return ret;
> +
> +	ret = isl76683_start_measurement(chip);
> +	if (ret < 0)
> +		return ret;
> +
> +	/* wait for measurement to complete */
> +	timeout = wait_for_completion_interruptible_timeout(
> +			&chip->irq_complete,
> +			msecs_to_jiffies(5000));
> +	if (timeout == 0) {
> +		dev_err(&chip->client->dev, "measurement timed out\n");
> +		return -ETIMEDOUT;
> +	} else if (timeout < 0) {
> +		dev_err(&chip->client->dev, "wait for measurement failed\n");
> +		return -EINTR;
> +	}
> +
> +	ret = isl76683_get_sensordata(chip, is_processed, val);
> +	if (ret) {
> +		dev_err(&chip->client->dev, "%s: Error %d reading lux\n",
> +				__func__, ret);
> +		return ret;
> +	}
> +
> +	return IIO_VAL_INT;
> +}
> +
> +static irqreturn_t isl76683_trigger_handler(int irq, void *p)
> +{
> +	struct iio_poll_func *pf = p;
> +	struct iio_dev *indio_dev = pf->indio_dev;
> +	struct isl76683_chip *chip = iio_priv(indio_dev);
> +	u8 data[16] = { }; /* 2x 8-bit data + padding + 8 bytes timestamp */
> +	__le16 buf;
> +	int ret;
> +
> +	ret = regmap_bulk_read(chip->rmp, ISL76683_REG_SENSOR_LSB, &buf,
> +			       sizeof(buf));
> +	if (ret)
> +		return IRQ_HANDLED;
> +
> +	data[0] = buf;

Only the bottom 8 bits?

> +
> +	iio_push_to_buffers_with_timestamp(indio_dev, data, chip->time_ns);
> +
> +	iio_trigger_notify_done(indio_dev->trig);
> +
> +	ret = isl76683_start_measurement(chip);
> +	if (ret < 0)
> +		return IRQ_HANDLED;

Not really worth checking ret :)  Anyhow, this will go away I think given
comments below.

> +
> +	return IRQ_HANDLED;
> +}
> +
> +static int isl76683_buffer_preenable(struct iio_dev *indio_dev)
> +{
> +	struct isl76683_chip *chip = iio_priv(indio_dev);
> +	int ret;
> +
> +	chip->photodiode = indio_dev->channels[0].channel2;
> +	ret = isl76683_set_config(chip);

This is configuring the device to match the scan mode requested.
Right callback for that is update scan_mode in struct iio_info.

> +
> +	return ret;
> +}
> +
> +static const struct iio_buffer_setup_ops isl76683_buffer_setup_ops = {
> +	.preenable = &isl76683_buffer_preenable,
> +	.predisable = iio_triggered_buffer_predisable,
> +	.postenable = iio_triggered_buffer_postenable,
> +	.validate_scan_mask = &iio_validate_scan_mask_onehot,
> +};
> +
> +static int isl76683_read_raw(struct iio_dev *indio_dev,
> +			     struct iio_chan_spec const *chan,
> +			     int *val, int *val2, long mask)
> +{
> +	struct isl76683_chip *chip = iio_priv(indio_dev);
> +	int ret;
> +
> +	switch (mask) {
> +	case IIO_CHAN_INFO_RAW:
> +		ret = iio_device_claim_direct_mode(indio_dev);
> +		if (ret)
> +			return ret;
> +		ret = isl76683_singleshot_conversion(chip, chan, false, val);
> +		iio_device_release_direct_mode(indio_dev);
> +		return ret;
> +	case IIO_CHAN_INFO_PROCESSED:
> +		ret = iio_device_claim_direct_mode(indio_dev);
> +		if (ret)
> +			return ret;
> +		ret = isl76683_singleshot_conversion(chip, chan, true, val);
> +		iio_device_release_direct_mode(indio_dev);
> +		return ret;
> +	case IIO_CHAN_INFO_SCALE:
> +		*val = isl76683_lux_ranges_available[chip->luxrange];
> +		return IIO_VAL_INT;
> +	}
> +
> +	return -EINVAL;
> +}
> +
> +static int isl76683_write_raw(struct iio_dev *indio_dev,
> +			     struct iio_chan_spec const *chan,
> +			     int val, int val2, long mask)
> +{
> +	struct isl76683_chip *chip = iio_priv(indio_dev);
> +	int ret;
> +
> +	switch (mask) {
> +	case IIO_CHAN_INFO_SCALE:
> +		mutex_lock(&chip->lock);
> +		chip->luxrange = find_closest(val,
> +			isl76683_lux_ranges_available,
> +			ARRAY_SIZE(isl76683_lux_ranges_available));
> +		ret = isl76683_set_config(chip);
> +		mutex_unlock(&chip->lock);
> +		return ret;
> +	}
> +
> +	return -EINVAL;
> +}
> +
> +static IIO_CONST_ATTR(in_illuminance_scale_available,
> +		ISL76683_LUXRANGE_STR);
> +
> +static struct attribute *isl76683_attributes[] = {
> +	&iio_const_attr_in_illuminance_scale_available.dev_attr.attr,
> +	NULL
> +};
> +
> +static const struct attribute_group isl76683_attribute_group = {
> +	.attrs = isl76683_attributes,
> +};
> +
> +#define ISL76683_CHANNEL(_ch2, _si) { \
> +	.type = IIO_LIGHT, \
> +	.modified = 1, \
> +	.channel2 = _ch2, \
> +	.info_mask_separate = BIT(IIO_CHAN_INFO_RAW) | \
> +			BIT(IIO_CHAN_INFO_PROCESSED), \

Channels should not be provided in both raw and processed forms.
The only time to provide processed data is when there is a reason
we can't provide the raw data (usually annoying non linear scaling etc).
So just provide the raw form here.

> +	.info_mask_shared_by_type = BIT(IIO_CHAN_INFO_SCALE), \
> +	.scan_index = _si, \
> +	.scan_type = { \
> +		.sign = 'u', \
> +		.realbits = 16, \
> +		.storagebits = 16, \
> +		.endianness = IIO_LE, \
> +	}, \
> +}
> +
> +static const struct iio_chan_spec isl76683_channels[] = {
> +	{
> +		.type = IIO_LIGHT,
> +		.info_mask_separate = BIT(IIO_CHAN_INFO_RAW) |
> +			BIT(IIO_CHAN_INFO_PROCESSED),

Again, provide raw only. Conversion up to userspace (which has
the advantage of floating point maths!)

> +		.info_mask_shared_by_type = BIT(IIO_CHAN_INFO_SCALE),
> +		.scan_index = 0,
> +		.scan_type = {
> +			.sign = 'u',
> +			.realbits = 16,
> +			.storagebits = 16,
> +			.endianness = IIO_LE,
> +		},
> +	},
> +	ISL76683_CHANNEL(IIO_MOD_LIGHT_IR, 1),
> +	ISL76683_CHANNEL(IIO_MOD_LIGHT_BOTH, 2),
> +	IIO_CHAN_SOFT_TIMESTAMP(3),
> +};
> +
> +static const struct iio_info isl76683_info = {
> +	.read_raw = isl76683_read_raw,
> +	.write_raw = isl76683_write_raw,
> +	.attrs = &isl76683_attribute_group,
> +};
> +
> +static int isl76683_set_trigger_state(struct iio_trigger *trig, bool enable)
> +{
> +	struct isl76683_chip *chip = iio_trigger_get_drvdata(trig);
> +	int ret;
> +
> +	if (enable) {
> +		chip->trig_enabled = true;

This is a little unusual.

Superficially it looks the device doesn't really have an internal sequencer, but
rather is triggered on demand and you are doing that in a fairly tight loop?

For that we have the generic iio-trig-loop.   Please look at whether
that can meet your usecase. It basically relies on using a thread to call
the irq thread of a device in a tight loop.

So the threaded part of the irq handler would do:
1. isl76683_start_measurement
2. Wait on completion
3. (completion from the actual interrupt handler)
4. grab data to buffer and return - then the loop trigger would restart
the sequence.

This makes it compatible with other triggers as well so a high resolution timer
trigger of sysfs trigger would work fine as well.

> +		ret = isl76683_start_measurement(chip);
> +		if (ret < 0)
> +			return ret;
> +	} else
> +		chip->trig_enabled = false;
> +
> +	return 0;
> +}
> +
> +static const struct iio_trigger_ops isl76683_trigger_ops = {
> +	.owner = THIS_MODULE,

This field is gone.  So drop the .owner from here.  If not I
can clean up when applying.   Note we dropped this in favour
of doing the equivalent using some macro magic.

> +	.set_trigger_state = isl76683_set_trigger_state,
> +	.validate_device = iio_trigger_validate_own_device,
> +};
> +
> +static int isl76683_probe(struct i2c_client *client,
> +			  const struct i2c_device_id *id)
> +{
> +	struct isl76683_chip *chip;
> +	struct iio_dev *indio_dev;
> +	struct device_node *np = client->dev.of_node;
> +	int rs = ISL76683_EXT_RESISTOR_DEFAULT;
> +	int v, ret;
> +
> +	indio_dev = devm_iio_device_alloc(&client->dev, sizeof(*chip));
> +	if (!indio_dev)
> +		return -ENOMEM;
> +
> +	chip = iio_priv(indio_dev);
> +
> +	i2c_set_clientdata(client, indio_dev);
> +	chip->client = client;
> +
> +	if (np) {
> +		ret = of_property_read_u32(np,
> +				"isil,external-resistor-kilo-ohms", &v);
> +		if (ret || v < ISL76683_KOHM_MIN || v > ISL76683_KOHM_MAX)
> +			dev_warn(&client->dev,
> +				"assuming %i kOhm resistor\n", rs);
> +		else
> +			rs = v;
> +	}
> +
> +	chip->luxrange = ISL76683_LUX_RANGE_DEFAULT;
> +	chip->external_resistor = rs;
> +	chip->photodiode = ISL76683_DIODE_DEFAULT;
> +
> +	chip->rmp = devm_regmap_init_i2c(client, &isl76683_regmap_config);
> +	if (IS_ERR(chip->rmp)) {
> +		ret = PTR_ERR(chip->rmp);
> +		dev_err(&client->dev, "%s: Error %d initializing regmap\n",
> +			__func__, ret);
> +		return ret;
> +	}
> +
> +	mutex_init(&chip->lock);
> +
> +	init_completion(&chip->irq_complete);
> +
> +	if (!client->irq) {
> +		dev_err(&client->dev, "no interrupt configured\n");
> +		return -EINVAL;
> +	}
> +
> +	indio_dev->dev.parent = &client->dev;
> +	indio_dev->info = &isl76683_info;
> +	indio_dev->channels = isl76683_channels;
> +	indio_dev->num_channels = ARRAY_SIZE(isl76683_channels);
> +	indio_dev->name = id->name;
> +	indio_dev->modes = INDIO_DIRECT_MODE;
> +
> +	chip->trig_enabled = false;
> +	chip->trig = devm_iio_trigger_alloc(&client->dev, "%s-dev%d",
> +			indio_dev->name, indio_dev->id);
> +	if (!chip->trig)
> +		return -ENOMEM;
> +
> +	chip->trig->ops = &isl76683_trigger_ops;
> +	chip->trig->dev.parent = &client->dev;
> +	iio_trigger_set_drvdata(chip->trig, chip);
> +
> +	ret = devm_request_irq(&client->dev, client->irq,
> +			isl76683_interrupt_handler,
> +			IRQF_TRIGGER_FALLING | IRQF_ONESHOT,
> +			"isl76683_event", indio_dev);
> +	if (ret) {
> +		dev_err(&client->dev, "irq request error\n");
> +		return ret;
> +	}
> +
> +	ret = devm_iio_trigger_register(&client->dev, chip->trig);
> +	if (ret) {
> +		dev_err(&client->dev, "iio_trigger register error\n");
> +		return ret;
> +	}
> +
> +	ret = isl76683_reset(chip);
> +	if (ret) {
> +		dev_err(&client->dev, "reset failed\n");
> +		return ret;
> +	}
> +
> +	ret = isl76683_get_sensordata(chip, false, &v);
> +	if (ret) {
> +		dev_err(&client->dev, "initial dummy readout failed\n");
> +		return ret;
> +	}
> +
> +	ret = devm_iio_triggered_buffer_setup(&client->dev, indio_dev, NULL,
> +			&isl76683_trigger_handler, &isl76683_buffer_setup_ops);
> +	if (ret)
> +		return ret;
> +
> +	ret = devm_iio_device_register(&client->dev, indio_dev);

This always rings alarm bells.  The moment we have a devm register
(which exposes the device to both in kernel and userspace consumers)
and we still have a remove function, that almost always means we
are looking at potential races.

The key thing is that the unregister for this will occur after the
remove function below.  Hence everything should still be working until
that point...  Given you turn the chip off in remove - that seems
rather unlikely!

Upshot, you can't use managed device registration, but need to
do use iio_device_register and iio_device_unregister to ensure the
ordering is correct.

> +	if (ret) {
> +		dev_err(&client->dev,
> +			"%s(): iio registration failed with error %d\n",
> +			__func__, ret);
> +	}
> +
> +	return ret;
> +}
> +
> +static int isl76683_remove(struct i2c_client *client)
> +{
> +	struct iio_dev *indio_dev = i2c_get_clientdata(client);
> +	struct isl76683_chip *chip = iio_priv(indio_dev);
> +
> +	pm_runtime_disable(&client->dev);
> +	pm_runtime_set_suspended(&client->dev);

So what form of pm runtime are you aiming for?  I'm guessing
that the intent was to do a power down on idle?  That needs
to be set up, otherwise this won't do runtime pm at all..

If the intent is just to allow for that support later, then
I'd prefer you actually just did standard suspend and
resume for now and then made the change to the runtime forms
when that support is ready.

> +	isl76683_power(chip, false);
> +
> +	return 0;
> +}
> +
> +#if defined(CONFIG_PM_SLEEP) || defined(CONFIG_PM)

On this the growing movement in the kernel is not to bother
with the ifdef fun, but instead mark these functions
as __maybe_unused.  The compiler will happily drop them
anyway as they aren't used.


> +static int isl76683_runtime_suspend(struct device *dev)
> +{
> +	struct isl76683_chip *chip =
> +		iio_priv(i2c_get_clientdata(to_i2c_client(dev)));
> +	int ret;
> +
> +	mutex_lock(&chip->lock);
> +	ret = isl76683_power(chip, false);
> +	regcache_mark_dirty(chip->rmp);
> +	mutex_unlock(&chip->lock);
> +
> +	return ret;
> +}
> +
> +static int isl76683_runtime_resume(struct device *dev)
> +{
> +	struct isl76683_chip *chip =
> +		iio_priv(i2c_get_clientdata(to_i2c_client(dev)));
> +	int ret;
> +
> +	mutex_lock(&chip->lock);
> +	ret = isl76683_power(chip, true);
> +	mutex_unlock(&chip->lock);
> +
> +	return ret;
> +}
> +#endif
> +
> +static const struct dev_pm_ops isl76683_pm_ops = {
> +	SET_SYSTEM_SLEEP_PM_OPS(pm_runtime_force_suspend,
> +				pm_runtime_force_resume)
> +	SET_RUNTIME_PM_OPS(isl76683_runtime_suspend,
> +			   isl76683_runtime_resume, NULL)
> +};
> +
> +static const struct i2c_device_id isl76683_id[] = {
> +	{"isl76683", 0},
> +	{}
> +};
> +MODULE_DEVICE_TABLE(i2c, isl76683_id);
> +
> +static const struct of_device_id isl76683_of_match[] = {
> +	{ .compatible = "isil,isl76683", },
> +	{ },
> +};
> +MODULE_DEVICE_TABLE(of, isl76683_of_match);
> +
> +static struct i2c_driver isl76683_driver = {
> +	.driver  = {
> +		.name = "isl76683",
> +		.of_match_table = isl76683_of_match,
> +		.pm = &isl76683_pm_ops,
> +	},
> +	.probe = isl76683_probe,
> +	.remove = isl76683_remove,
> +	.id_table = isl76683_id,
> +};
> +
> +module_i2c_driver(isl76683_driver);
> +
> +MODULE_DESCRIPTION("ISL76683 Ambient Light Sensor driver");
> +MODULE_LICENSE("GPL");
> +MODULE_AUTHOR("Christoph Fritz <chf.fritz@googlemail.com>");

--
To unsubscribe from this list: send the line "unsubscribe linux-iio" in
the body of a message to majordomo@vger.kernel.org
More majordomo info at  http://vger.kernel.org/majordomo-info.html

Patch

diff --git a/drivers/iio/light/Kconfig b/drivers/iio/light/Kconfig
index 2356ed9..4f0882c 100644
--- a/drivers/iio/light/Kconfig
+++ b/drivers/iio/light/Kconfig
@@ -194,6 +194,18 @@  config ISL29125
 	  To compile this driver as a module, choose M here: the module will be
 	  called isl29125.
 
+config ISL76683
+	tristate "Intersil ISL76683 light sensor"
+	depends on I2C
+	select IIO_BUFFER
+	select IIO_TRIGGERED_BUFFER
+	help
+	  Say Y here if you want to build a driver for the Intersil ISL76683
+	  light sensor for I2C.
+
+	  To compile this driver as a module, choose M here: the module will be
+	  called isl76683.
+
 config HID_SENSOR_ALS
 	depends on HID_SENSOR_HUB
 	select IIO_BUFFER
diff --git a/drivers/iio/light/Makefile b/drivers/iio/light/Makefile
index fa32fa4..886a51f 100644
--- a/drivers/iio/light/Makefile
+++ b/drivers/iio/light/Makefile
@@ -22,6 +22,7 @@  obj-$(CONFIG_HID_SENSOR_PROX)	+= hid-sensor-prox.o
 obj-$(CONFIG_SENSORS_ISL29018)	+= isl29018.o
 obj-$(CONFIG_SENSORS_ISL29028)	+= isl29028.o
 obj-$(CONFIG_ISL29125)		+= isl29125.o
+obj-$(CONFIG_ISL76683)		+= isl76683.o
 obj-$(CONFIG_JSA1212)		+= jsa1212.o
 obj-$(CONFIG_SENSORS_LM3533)	+= lm3533-als.o
 obj-$(CONFIG_LTR501)		+= ltr501.o
diff --git a/drivers/iio/light/isl76683.c b/drivers/iio/light/isl76683.c
new file mode 100644
index 0000000..4d158f6
--- /dev/null
+++ b/drivers/iio/light/isl76683.c
@@ -0,0 +1,663 @@ 
+/*
+ * IIO driver for the light sensor ISL76683.
+ *
+ * Copyright (c) 2017 Christoph Fritz <chf.fritz@googlemail.com>
+ *
+ * This program is free software; you can redistribute it and/or modify
+ * it under the terms of the GNU General Public License as published by
+ * the Free Software Foundation; either version 2 of the License, or
+ * (at your option) any later version.
+ *
+ * This program is distributed in the hope that it will be useful, but WITHOUT
+ * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
+ * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
+ * more details.
+ *
+ * Datasheet:
+ *  http://www.intersil.com/content/dam/Intersil/documents/isl7/isl76683.pdf
+ */
+
+#include <linux/module.h>
+#include <linux/i2c.h>
+#include <linux/pm_runtime.h>
+#include <linux/regmap.h>
+#include <linux/util_macros.h>
+#include <linux/iio/iio.h>
+#include <linux/iio/sysfs.h>
+#include <linux/iio/buffer.h>
+#include <linux/iio/trigger.h>
+#include <linux/iio/trigger_consumer.h>
+#include <linux/iio/triggered_buffer.h>
+
+#define ISL76683_REG_CMD		0x00
+#define ISL76683_REG_CTRL		0x01
+#define ISL76683_REG_THR_HI		0x02
+#define ISL76683_REG_THR_LO		0x03
+#define ISL76683_REG_SENSOR_LSB		0x04
+#define ISL76683_REG_SENSOR_MSB		0x05
+#define ISL76683_REG_CLEAR_INT		0x40
+#define ISL76683_REGMAP_MAX		0x40
+
+#define ISL76683_CMD_ENABLE		BIT(7)
+#define ISL76683_CMD_PWRDWN		BIT(6)
+#define ISL76683_WIDTH_MASK		0x3
+#define ISL76683_PHOTOD_SHFT		2
+#define ISL76683_PHOTOD_MASK		GENMASK(3, ISL76683_PHOTOD_SHFT)
+#define ISL76683_INTPERS_MASK		0x3
+#define ISL76683_LUXRANGE_SHFT		2
+#define ISL76683_LUXRANGE_MASK		(0x3 << ISL76683_LUXRANGE_SHFT)
+#define ISL76683_LUXRANGE_STR		"1000 4000 16000 64000"
+
+enum isl76683_dmode {
+	ISL76683_DIODE_0 = 0,
+	ISL76683_DIODE_IR,
+	ISL76683_DIODE_DIFF,
+};
+
+enum isl76683_lux_range {
+	ISL76683_LUX_1000 = 0,
+	ISL76683_LUX_4000,
+	ISL76683_LUX_16000,
+	ISL76683_LUX_64000,
+};
+
+static const int isl76683_lux_ranges_available[] = {
+	1000, 4000, 16000, 64000};
+
+#define ISL76683_LUX_RANGE_DEFAULT	ISL76683_LUX_1000
+#define ISL76683_DIODE_MAX		ISL76683_DIODE_DIFF
+#define ISL76683_DIODE_DEFAULT		ISL76683_DIODE_0
+#define ISL76683_WIDTH_DEFAULT		0x0
+#define ISL76683_RESOLUTION_DEFAULT	16
+#define ISL76683_EXT_RESISTOR_DEFAULT	100
+#define ISL76683_KOHM_MIN		1
+#define ISL76683_KOHM_MAX		1000
+#define ISL76683_INTPERS_DEFAULT	0x0
+#define ISL76683_THR_DEFAULT		0x7f
+
+struct isl76683_chip {
+	enum isl76683_lux_range	luxrange;
+	int			external_resistor;
+	enum isl76683_dmode	photodiode;
+	struct i2c_client	*client;
+	struct regmap		*rmp;
+	struct completion	irq_complete;
+	struct iio_trigger	*trig;
+	bool			trig_enabled;
+	struct mutex		lock;
+	s64			time_ns;
+};
+
+static bool isl76683_readable_reg(struct device *dev, unsigned int reg)
+{
+	switch (reg) {
+	case ISL76683_REG_CMD:
+	case ISL76683_REG_CTRL:
+	case ISL76683_REG_THR_HI:
+	case ISL76683_REG_THR_LO:
+	case ISL76683_REG_SENSOR_LSB:
+	case ISL76683_REG_SENSOR_MSB:
+	case ISL76683_REG_CLEAR_INT:
+		return true;
+	default:
+		return false;
+	}
+}
+
+static bool isl76683_writeable_reg(struct device *dev, unsigned int reg)
+{
+	switch (reg) {
+	case ISL76683_REG_CMD:
+	case ISL76683_REG_CTRL:
+	case ISL76683_REG_THR_HI:
+	case ISL76683_REG_THR_LO:
+		return true;
+	default:
+		return false;
+	}
+}
+
+static bool isl76683_is_volatile_reg(struct device *dev, unsigned int reg)
+{
+	switch (reg) {
+	case ISL76683_REG_SENSOR_LSB:
+	case ISL76683_REG_SENSOR_MSB:
+	case ISL76683_REG_CLEAR_INT:
+		return true;
+	default:
+		return false;
+	}
+}
+
+static const struct regmap_config isl76683_regmap_config = {
+	.reg_bits = 8,
+	.val_bits = 8,
+	.max_register = ISL76683_REGMAP_MAX,
+	.readable_reg = isl76683_readable_reg,
+	.writeable_reg = isl76683_writeable_reg,
+	.volatile_reg = isl76683_is_volatile_reg,
+	.cache_type = REGCACHE_RBTREE,
+};
+
+static int isl76683_set_config(struct isl76683_chip *chip)
+{
+	int ret;
+
+	ret = regmap_update_bits(chip->rmp, ISL76683_REG_CTRL,
+				ISL76683_LUXRANGE_MASK | ISL76683_INTPERS_MASK,
+				(chip->luxrange << ISL76683_LUXRANGE_SHFT) |
+				ISL76683_INTPERS_DEFAULT);
+	if (ret < 0)
+		return ret;
+
+	ret = regmap_update_bits(chip->rmp, ISL76683_REG_CMD,
+				ISL76683_PHOTOD_MASK | ISL76683_WIDTH_MASK,
+				(chip->photodiode << ISL76683_PHOTOD_SHFT) |
+				ISL76683_WIDTH_DEFAULT);
+	if (ret < 0)
+		return ret;
+
+	ret = regmap_write(chip->rmp, ISL76683_REG_THR_HI,
+				ISL76683_THR_DEFAULT);
+	if (ret < 0)
+		return ret;
+
+	return regmap_write(chip->rmp, ISL76683_REG_THR_LO,
+				ISL76683_THR_DEFAULT);
+}
+
+static int isl76683_power(struct isl76683_chip *chip, bool on)
+{
+	int ret;
+
+	ret = regmap_update_bits(chip->rmp, ISL76683_REG_CMD,
+				ISL76683_CMD_ENABLE | ISL76683_CMD_PWRDWN,
+				0x0);
+	if (ret < 0)
+		return ret;
+
+	ret = regmap_update_bits(chip->rmp, ISL76683_REG_CMD,
+				ISL76683_CMD_ENABLE | ISL76683_CMD_PWRDWN,
+				on ? ISL76683_CMD_ENABLE : ISL76683_CMD_PWRDWN);
+	if (ret < 0)
+		return ret;
+
+	return on ? isl76683_set_config(chip) : 0;
+}
+
+static int isl76683_reset(struct isl76683_chip *chip)
+{
+	int ret;
+
+	ret = isl76683_power(chip, false);
+	if (ret < 0)
+		return ret;
+
+	return isl76683_power(chip, true);
+}
+
+static int isl76683_get_sensordata(struct isl76683_chip *chip,
+			     bool is_processed, int *val)
+{
+	unsigned int sensor_data, range, fsr;
+	__le16 sensor_raw;
+	int ret;
+
+	ret = regmap_bulk_read(chip->rmp, ISL76683_REG_SENSOR_LSB,
+				&sensor_raw, sizeof(sensor_raw));
+	if (ret)
+		return ret;
+
+	sensor_data = le16_to_cpu(sensor_raw);
+
+	if (!is_processed) {
+		*val = sensor_data;
+		return 0;
+	}
+
+	/* range values taken from datasheet (table 9) */
+	if (chip->luxrange == ISL76683_LUX_1000)
+		range = 973;
+	else if (chip->luxrange == ISL76683_LUX_4000)
+		range = 3892;
+	else if (chip->luxrange == ISL76683_LUX_16000)
+		range = 15568;
+	else if (chip->luxrange == ISL76683_LUX_64000)
+		range = 62272;
+	else
+		return -EINVAL;
+
+	/* equations from datasheet (EQ.3 and EQ.4) */
+	fsr = (100 * range) / chip->external_resistor;
+	*val = (fsr * sensor_data) / (1 << ISL76683_RESOLUTION_DEFAULT);
+
+	return 0;
+}
+
+static irqreturn_t isl76683_interrupt_handler(int irq, void *private)
+{
+	struct iio_dev *indio_dev = private;
+	struct isl76683_chip *chip = iio_priv(indio_dev);
+
+	chip->time_ns = iio_get_time_ns(indio_dev);
+
+	if (chip->trig_enabled)
+		iio_trigger_poll(chip->trig);
+
+	if (!completion_done(&chip->irq_complete))
+		complete(&chip->irq_complete);
+
+	return IRQ_HANDLED;
+}
+
+static inline int isl76683_start_measurement(struct isl76683_chip *chip)
+{
+	int dummy;
+
+	/* dummy read is clearing irq and triggers new measurement */
+	return regmap_read(chip->rmp, ISL76683_REG_CLEAR_INT, &dummy);
+}
+
+static int isl76683_singleshot_conversion(struct isl76683_chip *chip,
+					  const struct iio_chan_spec *chan,
+					  bool is_processed, int *val)
+{
+	long timeout;
+	int ret;
+
+	switch (chan->channel2) {
+	case IIO_MOD_LIGHT_BOTH:
+		chip->photodiode = ISL76683_DIODE_DIFF;
+		break;
+	case IIO_MOD_LIGHT_IR:
+		chip->photodiode = ISL76683_DIODE_IR;
+		break;
+	default:
+		chip->photodiode = ISL76683_DIODE_0;
+	}
+
+	ret = isl76683_set_config(chip);
+	if (ret)
+		return ret;
+
+	ret = isl76683_start_measurement(chip);
+	if (ret < 0)
+		return ret;
+
+	/* wait for measurement to complete */
+	timeout = wait_for_completion_interruptible_timeout(
+			&chip->irq_complete,
+			msecs_to_jiffies(5000));
+	if (timeout == 0) {
+		dev_err(&chip->client->dev, "measurement timed out\n");
+		return -ETIMEDOUT;
+	} else if (timeout < 0) {
+		dev_err(&chip->client->dev, "wait for measurement failed\n");
+		return -EINTR;
+	}
+
+	ret = isl76683_get_sensordata(chip, is_processed, val);
+	if (ret) {
+		dev_err(&chip->client->dev, "%s: Error %d reading lux\n",
+				__func__, ret);
+		return ret;
+	}
+
+	return IIO_VAL_INT;
+}
+
+static irqreturn_t isl76683_trigger_handler(int irq, void *p)
+{
+	struct iio_poll_func *pf = p;
+	struct iio_dev *indio_dev = pf->indio_dev;
+	struct isl76683_chip *chip = iio_priv(indio_dev);
+	u8 data[16] = { }; /* 2x 8-bit data + padding + 8 bytes timestamp */
+	__le16 buf;
+	int ret;
+
+	ret = regmap_bulk_read(chip->rmp, ISL76683_REG_SENSOR_LSB, &buf,
+			       sizeof(buf));
+	if (ret)
+		return IRQ_HANDLED;
+
+	data[0] = buf;
+
+	iio_push_to_buffers_with_timestamp(indio_dev, data, chip->time_ns);
+
+	iio_trigger_notify_done(indio_dev->trig);
+
+	ret = isl76683_start_measurement(chip);
+	if (ret < 0)
+		return IRQ_HANDLED;
+
+	return IRQ_HANDLED;
+}
+
+static int isl76683_buffer_preenable(struct iio_dev *indio_dev)
+{
+	struct isl76683_chip *chip = iio_priv(indio_dev);
+	int ret;
+
+	chip->photodiode = indio_dev->channels[0].channel2;
+	ret = isl76683_set_config(chip);
+
+	return ret;
+}
+
+static const struct iio_buffer_setup_ops isl76683_buffer_setup_ops = {
+	.preenable = &isl76683_buffer_preenable,
+	.predisable = iio_triggered_buffer_predisable,
+	.postenable = iio_triggered_buffer_postenable,
+	.validate_scan_mask = &iio_validate_scan_mask_onehot,
+};
+
+static int isl76683_read_raw(struct iio_dev *indio_dev,
+			     struct iio_chan_spec const *chan,
+			     int *val, int *val2, long mask)
+{
+	struct isl76683_chip *chip = iio_priv(indio_dev);
+	int ret;
+
+	switch (mask) {
+	case IIO_CHAN_INFO_RAW:
+		ret = iio_device_claim_direct_mode(indio_dev);
+		if (ret)
+			return ret;
+		ret = isl76683_singleshot_conversion(chip, chan, false, val);
+		iio_device_release_direct_mode(indio_dev);
+		return ret;
+	case IIO_CHAN_INFO_PROCESSED:
+		ret = iio_device_claim_direct_mode(indio_dev);
+		if (ret)
+			return ret;
+		ret = isl76683_singleshot_conversion(chip, chan, true, val);
+		iio_device_release_direct_mode(indio_dev);
+		return ret;
+	case IIO_CHAN_INFO_SCALE:
+		*val = isl76683_lux_ranges_available[chip->luxrange];
+		return IIO_VAL_INT;
+	}
+
+	return -EINVAL;
+}
+
+static int isl76683_write_raw(struct iio_dev *indio_dev,
+			     struct iio_chan_spec const *chan,
+			     int val, int val2, long mask)
+{
+	struct isl76683_chip *chip = iio_priv(indio_dev);
+	int ret;
+
+	switch (mask) {
+	case IIO_CHAN_INFO_SCALE:
+		mutex_lock(&chip->lock);
+		chip->luxrange = find_closest(val,
+			isl76683_lux_ranges_available,
+			ARRAY_SIZE(isl76683_lux_ranges_available));
+		ret = isl76683_set_config(chip);
+		mutex_unlock(&chip->lock);
+		return ret;
+	}
+
+	return -EINVAL;
+}
+
+static IIO_CONST_ATTR(in_illuminance_scale_available,
+		ISL76683_LUXRANGE_STR);
+
+static struct attribute *isl76683_attributes[] = {
+	&iio_const_attr_in_illuminance_scale_available.dev_attr.attr,
+	NULL
+};
+
+static const struct attribute_group isl76683_attribute_group = {
+	.attrs = isl76683_attributes,
+};
+
+#define ISL76683_CHANNEL(_ch2, _si) { \
+	.type = IIO_LIGHT, \
+	.modified = 1, \
+	.channel2 = _ch2, \
+	.info_mask_separate = BIT(IIO_CHAN_INFO_RAW) | \
+			BIT(IIO_CHAN_INFO_PROCESSED), \
+	.info_mask_shared_by_type = BIT(IIO_CHAN_INFO_SCALE), \
+	.scan_index = _si, \
+	.scan_type = { \
+		.sign = 'u', \
+		.realbits = 16, \
+		.storagebits = 16, \
+		.endianness = IIO_LE, \
+	}, \
+}
+
+static const struct iio_chan_spec isl76683_channels[] = {
+	{
+		.type = IIO_LIGHT,
+		.info_mask_separate = BIT(IIO_CHAN_INFO_RAW) |
+			BIT(IIO_CHAN_INFO_PROCESSED),
+		.info_mask_shared_by_type = BIT(IIO_CHAN_INFO_SCALE),
+		.scan_index = 0,
+		.scan_type = {
+			.sign = 'u',
+			.realbits = 16,
+			.storagebits = 16,
+			.endianness = IIO_LE,
+		},
+	},
+	ISL76683_CHANNEL(IIO_MOD_LIGHT_IR, 1),
+	ISL76683_CHANNEL(IIO_MOD_LIGHT_BOTH, 2),
+	IIO_CHAN_SOFT_TIMESTAMP(3),
+};
+
+static const struct iio_info isl76683_info = {
+	.read_raw = isl76683_read_raw,
+	.write_raw = isl76683_write_raw,
+	.attrs = &isl76683_attribute_group,
+};
+
+static int isl76683_set_trigger_state(struct iio_trigger *trig, bool enable)
+{
+	struct isl76683_chip *chip = iio_trigger_get_drvdata(trig);
+	int ret;
+
+	if (enable) {
+		chip->trig_enabled = true;
+		ret = isl76683_start_measurement(chip);
+		if (ret < 0)
+			return ret;
+	} else
+		chip->trig_enabled = false;
+
+	return 0;
+}
+
+static const struct iio_trigger_ops isl76683_trigger_ops = {
+	.owner = THIS_MODULE,
+	.set_trigger_state = isl76683_set_trigger_state,
+	.validate_device = iio_trigger_validate_own_device,
+};
+
+static int isl76683_probe(struct i2c_client *client,
+			  const struct i2c_device_id *id)
+{
+	struct isl76683_chip *chip;
+	struct iio_dev *indio_dev;
+	struct device_node *np = client->dev.of_node;
+	int rs = ISL76683_EXT_RESISTOR_DEFAULT;
+	int v, ret;
+
+	indio_dev = devm_iio_device_alloc(&client->dev, sizeof(*chip));
+	if (!indio_dev)
+		return -ENOMEM;
+
+	chip = iio_priv(indio_dev);
+
+	i2c_set_clientdata(client, indio_dev);
+	chip->client = client;
+
+	if (np) {
+		ret = of_property_read_u32(np,
+				"isil,external-resistor-kilo-ohms", &v);
+		if (ret || v < ISL76683_KOHM_MIN || v > ISL76683_KOHM_MAX)
+			dev_warn(&client->dev,
+				"assuming %i kOhm resistor\n", rs);
+		else
+			rs = v;
+	}
+
+	chip->luxrange = ISL76683_LUX_RANGE_DEFAULT;
+	chip->external_resistor = rs;
+	chip->photodiode = ISL76683_DIODE_DEFAULT;
+
+	chip->rmp = devm_regmap_init_i2c(client, &isl76683_regmap_config);
+	if (IS_ERR(chip->rmp)) {
+		ret = PTR_ERR(chip->rmp);
+		dev_err(&client->dev, "%s: Error %d initializing regmap\n",
+			__func__, ret);
+		return ret;
+	}
+
+	mutex_init(&chip->lock);
+
+	init_completion(&chip->irq_complete);
+
+	if (!client->irq) {
+		dev_err(&client->dev, "no interrupt configured\n");
+		return -EINVAL;
+	}
+
+	indio_dev->dev.parent = &client->dev;
+	indio_dev->info = &isl76683_info;
+	indio_dev->channels = isl76683_channels;
+	indio_dev->num_channels = ARRAY_SIZE(isl76683_channels);
+	indio_dev->name = id->name;
+	indio_dev->modes = INDIO_DIRECT_MODE;
+
+	chip->trig_enabled = false;
+	chip->trig = devm_iio_trigger_alloc(&client->dev, "%s-dev%d",
+			indio_dev->name, indio_dev->id);
+	if (!chip->trig)
+		return -ENOMEM;
+
+	chip->trig->ops = &isl76683_trigger_ops;
+	chip->trig->dev.parent = &client->dev;
+	iio_trigger_set_drvdata(chip->trig, chip);
+
+	ret = devm_request_irq(&client->dev, client->irq,
+			isl76683_interrupt_handler,
+			IRQF_TRIGGER_FALLING | IRQF_ONESHOT,
+			"isl76683_event", indio_dev);
+	if (ret) {
+		dev_err(&client->dev, "irq request error\n");
+		return ret;
+	}
+
+	ret = devm_iio_trigger_register(&client->dev, chip->trig);
+	if (ret) {
+		dev_err(&client->dev, "iio_trigger register error\n");
+		return ret;
+	}
+
+	ret = isl76683_reset(chip);
+	if (ret) {
+		dev_err(&client->dev, "reset failed\n");
+		return ret;
+	}
+
+	ret = isl76683_get_sensordata(chip, false, &v);
+	if (ret) {
+		dev_err(&client->dev, "initial dummy readout failed\n");
+		return ret;
+	}
+
+	ret = devm_iio_triggered_buffer_setup(&client->dev, indio_dev, NULL,
+			&isl76683_trigger_handler, &isl76683_buffer_setup_ops);
+	if (ret)
+		return ret;
+
+	ret = devm_iio_device_register(&client->dev, indio_dev);
+	if (ret) {
+		dev_err(&client->dev,
+			"%s(): iio registration failed with error %d\n",
+			__func__, ret);
+	}
+
+	return ret;
+}
+
+static int isl76683_remove(struct i2c_client *client)
+{
+	struct iio_dev *indio_dev = i2c_get_clientdata(client);
+	struct isl76683_chip *chip = iio_priv(indio_dev);
+
+	pm_runtime_disable(&client->dev);
+	pm_runtime_set_suspended(&client->dev);
+	isl76683_power(chip, false);
+
+	return 0;
+}
+
+#if defined(CONFIG_PM_SLEEP) || defined(CONFIG_PM)
+static int isl76683_runtime_suspend(struct device *dev)
+{
+	struct isl76683_chip *chip =
+		iio_priv(i2c_get_clientdata(to_i2c_client(dev)));
+	int ret;
+
+	mutex_lock(&chip->lock);
+	ret = isl76683_power(chip, false);
+	regcache_mark_dirty(chip->rmp);
+	mutex_unlock(&chip->lock);
+
+	return ret;
+}
+
+static int isl76683_runtime_resume(struct device *dev)
+{
+	struct isl76683_chip *chip =
+		iio_priv(i2c_get_clientdata(to_i2c_client(dev)));
+	int ret;
+
+	mutex_lock(&chip->lock);
+	ret = isl76683_power(chip, true);
+	mutex_unlock(&chip->lock);
+
+	return ret;
+}
+#endif
+
+static const struct dev_pm_ops isl76683_pm_ops = {
+	SET_SYSTEM_SLEEP_PM_OPS(pm_runtime_force_suspend,
+				pm_runtime_force_resume)
+	SET_RUNTIME_PM_OPS(isl76683_runtime_suspend,
+			   isl76683_runtime_resume, NULL)
+};
+
+static const struct i2c_device_id isl76683_id[] = {
+	{"isl76683", 0},
+	{}
+};
+MODULE_DEVICE_TABLE(i2c, isl76683_id);
+
+static const struct of_device_id isl76683_of_match[] = {
+	{ .compatible = "isil,isl76683", },
+	{ },
+};
+MODULE_DEVICE_TABLE(of, isl76683_of_match);
+
+static struct i2c_driver isl76683_driver = {
+	.driver  = {
+		.name = "isl76683",
+		.of_match_table = isl76683_of_match,
+		.pm = &isl76683_pm_ops,
+	},
+	.probe = isl76683_probe,
+	.remove = isl76683_remove,
+	.id_table = isl76683_id,
+};
+
+module_i2c_driver(isl76683_driver);
+
+MODULE_DESCRIPTION("ISL76683 Ambient Light Sensor driver");
+MODULE_LICENSE("GPL");
+MODULE_AUTHOR("Christoph Fritz <chf.fritz@googlemail.com>");

