//------------------------------------------------------------------------------
// speedcalculator.h
//------------------------------------------------------------------------------
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
// 02110-1301, USA.
//
//------------------------------------------------------------------------------
// Copyright (C) 2010 "Zalewa" <zalewapl@gmail.com>
//------------------------------------------------------------------------------
#ifndef __SPEEDCALCULATOR_H__
#define __SPEEDCALCULATOR_H__

#include <QVector>
#include <QTime>

class SpeedCalculator
{
	public:
		SpeedCalculator();

		/**
		 *	@brief In seconds.
		 *
		 *	@return A negative value is returned if time cannot be calculated
		 *	properly.
		 */
		float						estimatedTimeUntilArrival() const;

		unsigned					expectedDataSize() const { return dataSizeExpected; }

		/**
		 *	@brief In bytes per second.
		 */
		float						getSpeed() const;

		int							maxResolution() const { return _maxResolution; }

		void						registerDataAmount(unsigned totalAmountOfArrivedData);

		void						setExpectedDataSize(unsigned size);

		/**
		 *	@brief The maximum amount of data that can be stored in
		 *	arrivalData vector.
		 *
		 *	Values below 2 are not allowed here and will be automatically
		 *	changed to 2.
		 *	This is the number that will be also used to calculate mean in
		 *	getSpeed() method. The higher the number the more accurate the
		 *	result but also the longer it is required to wait to fill the entire
		 *	vector.
		 *
		 *	If a new data is registered and the vector's size is already as big
		 *	as the maxResolution value the oldest data will be discarded.
		 */
		void						setMaxResolution(int max);

		/**
		 * @brief Clears all values. Prepares SpeedCalculator for new speed
		 * measure.
		 *
		 * This will clear arrivalData and dataSizeExpected variables and
		 * restart the clock.
		 */
		void						start();

	protected:
		class DataArrivalInfo
		{
            public:
                /**
                 *	@brief In bytes.
                 */
                unsigned				totalAmountOfArrivedData;

                /**
                 *	@brief In miliseconds.
                 */
                unsigned				timeOfArrival;

                DataArrivalInfo()
                {
                    this->totalAmountOfArrivedData = 0;
                    this->timeOfArrival = 0;
                }

                DataArrivalInfo(unsigned totalAmountOfArrivedData, unsigned timeOfArrival)
                {
                    this->totalAmountOfArrivedData = totalAmountOfArrivedData;
                    this->timeOfArrival = timeOfArrival;
                }
		};

		QVector<DataArrivalInfo>	arrivalData;
		QTime						clock;
		unsigned					dataSizeExpected;

		/// Default value is 50.
		int							_maxResolution;


		unsigned					maxTimeDifference() const;
};

#endif
