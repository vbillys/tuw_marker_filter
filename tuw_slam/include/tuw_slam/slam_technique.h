#ifndef SLAM_TECHNIQUE_H
#define SLAM_TECHNIQUE_H

#include <tuw_geometry/tuw_geometry.h>
#include <opencv2/core/core.hpp>
#include <boost/date_time/posix_time/ptime.hpp>

namespace tuw {

/**
 * Base class for SLAM techniques
 */
class SLAMTechnique;
typedef std::shared_ptr< SLAMTechnique > SLAMTechniquePtr;
typedef std::shared_ptr< SLAMTechnique const> SLAMTechniqueConstPtr;

class SLAMTechnique {
public:
    enum Type {
        EKF = 0,
    };
    static std::map<Type, std::string> TypeName_;
    /**
     * Constructor
     * @param type used to identify the SLAM technique
     * @param orientation use orientation of feautures and landmarks
     **/
    SLAMTechnique ( Type type );
    /**
     * set a reset flag to reset the system on the next loop
     * @see SLAMTechnique::reset_ 
     **/
    void reset ( );
    /**
     * Returns the SLAM technique as enum
     * @returns enum
     * @see SLAMTechnique::Type
     **/
    Type getType() const;
    /**
     * Returns the SLAM technique as name
     * @returns name
     * @see SLAMTechnique::Type
     **/
    const std::string getTypeName() const;
    /**
     * Time of the last processed measurment
     * @returns time
     **/
    const boost::posix_time::ptime& time_last_update() const;
    /**
     * virtual function to set the config parameters
     * @param config point to a autogenerated ros reconfiguration config
     **/
    virtual void setConfig ( const void *config ) = 0;
    /**
     * starts the SLAM cycle and predicts the vehicles and landmark poses at the timestamp encoded into the measurement
     * @param yt TODO
     * @param C_Yt TODO
     * @param ut current control command
     * @param zt measurment with a timestamp
     **/
    virtual void cycle ( std::vector<Pose2D> &yt, cv::Mat_<double> &C_Yt, const Command &ut, const MeasurementConstPtr &zt ) = 0;
protected:
    /**
     * Inits the system
     **/
    virtual void init ( ) = 0;
    /**
     * updates the timestamp_last_update_ and the duration_last_update_
     * on the first call it will set the timestamp_last_update_ to t and the duration_last_update_ to zero
     * the duration_last_update_ will be used for the prediction step
     * @return true on sussesful update, false on first use and if t is in the past
     **/
    bool updateTimestamp(const boost::posix_time::ptime& t);
    bool reset_;                                            /// on true the system should be reseted on the next loop
    boost::posix_time::ptime timestamp_last_update_;        /// time of the last processed measurment
    boost::posix_time::time_duration duration_last_update_; /// time since the previous processed measurment
private:  
    Type type_;                                             /// used SLAM technique
};
};

#endif // SLAM_TECHNIQUE_H
