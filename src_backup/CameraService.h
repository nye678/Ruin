#ifndef __RUIN_CAMERASERVICE_H_
#define __RUIN_CAMERASERVICE_H_

#include "Camera.h"
#include "Singleton.h"

namespace Ruin
{
	class CameraService
	{
		friend class Singleton<CameraService>;

		Camera _current;
	public:
		void setCurrent(const Camera &);

		Camera getCurrent();

	private:
		CameraService();
		CameraService(const CameraService &) = delete;
		CameraService &operator= (const CameraService &) = delete;
	};
}

#endif