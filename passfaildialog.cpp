#include <QtDebug>
#include <iostream>
#include "passfaildialog.h"
#include "ui_passfaildialog.h"

PassFailDialog::PassFailDialog(PassFailSettings& settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PassFailDialog)
{
    ui->setupUi(this);
    m_settings = settings;
    Init();
}

PassFailDialog::~PassFailDialog()
{
    // delete all the items that were allocated
    delete ui;
}

void PassFailDialog::Cleanup()
{
    //
    // Free all the allocated items
    //
    if (ui != nullptr && ui->treeWidget != nullptr)
    {
        for (int i = 0; i < ui->treeWidget->topLevelItemCount(); ++i)
        {
            QTreeWidgetItem *parent = ui->treeWidget->topLevelItem(i);

            for (int n = 0; n < parent->childCount(); ++n)
            {
                delete parent->child(n);    // QTreeWidgetItem destructor removes item from the tree for us
            }

            delete parent;
        }
    }
}

void PassFailDialog::Init()
{
    QStringList header = (QStringList() << "Property" << "Value");
    ui->treeWidget->setColumnCount(3);  // Property  Value  Description (description column is hidden)
    ui->treeWidget->setHeaderLabels(header);
    ui->treeWidget->setColumnHidden(2, true);   // hide the item description column-- the descrption is displayed at the bottom of the dialog
    ui->treeWidget->setSortingEnabled(false);

    if (m_settings.sfrplus.b_enable)
    {
        QTreeWidgetItem *sfrplus  = new QTreeWidgetItem();
        sfrplus->setText(0, m_settings.sfrplus.name.c_str());
        sfrplus->setDisabled(false);

        ui->treeWidget->addTopLevelItem(sfrplus);
        ui->treeWidget->setCurrentItem(sfrplus);

        AddSubMinMaxEntry(sfrplus, m_settings.sfrplus.Chart_mean_pixel_level_bounds);
        AddSubEntry(sfrplus, m_settings.sfrplus.Chart_radial_pixel_shift_max);
        AddSubEntry(sfrplus, m_settings.sfrplus.Low_pixel_saturation_fraction_max);
        AddSubEntry(sfrplus, m_settings.sfrplus.High_pixel_saturation_fraction_max);
        AddSubEntry(sfrplus, m_settings.sfrplus.MTF50P_ratio_min);
        AddSubEntry(sfrplus, m_settings.sfrplus.MTF50P_CP_weighted_mean_min);
        AddSubEntry(sfrplus, m_settings.sfrplus.Rotation_degrees_max);
        AddSubEntry(sfrplus, m_settings.sfrplus.FOV_degrees_diagonal_min);
        AddSubEntry(sfrplus, m_settings.sfrplus.Convergence_angle_max);
        AddSubEntry(sfrplus, m_settings.sfrplus.Secondary_readout_1_center_mean_min);
        AddSubEntry(sfrplus, m_settings.sfrplus.Secondary_readout_1_outer_mean_min);
        AddSubEntry(sfrplus, m_settings.sfrplus.Secondary_readout_1_outer_min_min);
        AddSubEntry(sfrplus, m_settings.sfrplus.Secondary_readout_1_outer_quadrant_delta_max);
        AddSubEntry(sfrplus, m_settings.sfrplus.Secondary_readout_1_outer_quadrant_mean_min_min);
        AddSubEntry(sfrplus, m_settings.sfrplus.Secondary_readout_2_center_mean_min);
        AddSubEntry(sfrplus, m_settings.sfrplus.Secondary_readout_2_outer_mean_min);
        AddSubEntry(sfrplus, m_settings.sfrplus.Secondary_readout_2_outer_min_min);
        AddSubEntry(sfrplus, m_settings.sfrplus.Secondary_readout_2_outer_quadrant_delta_max);
        AddSubEntry(sfrplus, m_settings.sfrplus.Secondary_readout_2_outer_quadrant_mean_min_min);
        AddSubEntry(sfrplus, m_settings.sfrplus.Horizontal_bars_OK_min);
        AddSubEntry(sfrplus, m_settings.sfrplus.All_Edge_IDs_detected);
        AddSubEntry(sfrplus, m_settings.sfrplus.Mirrored_chart);
        AddSubEntry(sfrplus, m_settings.sfrplus.Bayer_decode);
        AddSubEntry(sfrplus, m_settings.sfrplus.Color_expected_detected);
        AddSubEntry(sfrplus, m_settings.sfrplus.DeltaE_00_mean_max);
        AddSubEntry(sfrplus, m_settings.sfrplus.Stepchart_expected_detected);
        AddSubEntry(sfrplus, m_settings.sfrplus.upside_down);
        AddSubEntry(sfrplus, m_settings.sfrplus.passfail_ini_file_date);
    }

    ///////////////////////////////////////////////////////
    // Add the Blemish entries to the CMFCGridPropertyCtrl
    ///////////////////////////////////////////////////////
    if (m_settings.blemish.b_enable)
    {
        QTreeWidgetItem *blemish  = new QTreeWidgetItem();
        blemish->setText(0, m_settings.blemish.name.c_str());
        blemish->setDisabled(false);
        ui->treeWidget->addTopLevelItem(blemish);

        AddSubEntry(blemish, m_settings.blemish.Dead_pixels_max);
        AddSubEntry(blemish, m_settings.blemish.Dead_pixel_clusters_max);
        AddSubEntry(blemish, m_settings.blemish.Defective_pixels_max_count);
        AddSubEntry(blemish, m_settings.blemish.Hot_pixel_clusters_max);
        AddSubEntry(blemish, m_settings.blemish.Hot_pixels_max);
        AddSubEntry(blemish, m_settings.blemish.Optical_center_offset_max);
        AddSubEntry(blemish, m_settings.blemish.Optical_center_offset_X_max);
        AddSubEntry(blemish, m_settings.blemish.Optical_center_offset_Y_max);
        AddSubEntry(blemish, m_settings.blemish.Relative_illumination_corner_diff_pct_max);
        AddSubEntry(blemish, m_settings.blemish.Relative_illumination_worst_corner_pct_min);
        AddSubEntry(blemish, m_settings.blemish.Uniformity_RoverG_corners_pct_max);
        AddSubEntry(blemish, m_settings.blemish.Uniformity_BoverG_corners_pct_max);
        AddSubVecEntry(blemish,m_settings.blemish.Blemish_size_pixels);
        AddSubVecEntry(blemish,m_settings.blemish.Blemish_maximum_count);
    }

    ///////////////////////////////////////////////////////
    // Add the OIS entries to the CMFCGridPropertyCtrl
    ///////////////////////////////////////////////////////
    if (m_settings.ois.b_enable)
    {
        QTreeWidgetItem *ois  = new QTreeWidgetItem();
        ois->setText(0, m_settings.ois.name.c_str());
        ois->setDisabled(false);
        ui->treeWidget->addTopLevelItem(ois);

        AddSubEntry(ois, m_settings.ois.L_MTF50_delta2_gain_summary_all_dB_min);
        AddSubEntry(ois, m_settings.ois.R_improve_H_dB_min);
        AddSubEntry(ois, m_settings.ois.R_improve_V_dB_min);
        AddSubEntry(ois, m_settings.ois.R_improve_ALL_dB_min);
    }

    ///////////////////////////////////////////////////////
    // Add the other entries to the CMFCGridPropertyCtrl
    ///////////////////////////////////////////////////////

    if ( m_settings.other.b_enable)
    {
        QTreeWidgetItem *other  = new QTreeWidgetItem(ui->treeWidget);
        other->setText(0, m_settings.other.name.c_str());
        other->setDisabled(false);
        ui->treeWidget->addTopLevelItem(other);

        for ( std::size_t idx = 0; idx < m_settings.other.ent_vec.size(); ++idx)
        {
            AddSubEntry(other, m_settings.other.ent_vec[idx]);
        }
    }
}

///
/// Conditionally adds an entry<T> as a subitem of a QTreeWidgetItem if the entry<T> is used in the pass/fail file
///
template <typename T> bool PassFailDialog::AddSubEntry(QTreeWidgetItem *parent, entry<T>& ent)
{
    bool result = false;

 //   if (ent.b_isUsed)
    {
        QString name  = ent.name.c_str();
#if 1
        //
        // Convert the value to a string.  Originally I was using QString::number(), but that doesn't work
        // if the data type is a string.  I tried storing the value in a QVariant, but there's no QVariant
        // constructor for a long.  To get around all this, there is a separate function to convert each
        // entry data type (so far we have long, double, string) to a string.
        //
        QString value = value_to_str(ent.value); // having T be a string complicates things, and we can't use
#else
        QString value = QString::number();
#endif

        QString description = ent.description.c_str();
        QStringList strings = (QStringList() << name << value << description);
        QTreeWidgetItem *item = new QTreeWidgetItem(strings);
        item->setHidden(false);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        parent->addChild(item);
        result = true;
    }

    return result;
}

///
/// Conditionally adds a minMaxEntry<T> as a subitem of a QTreeWidgetItem if the minMaxEntry<T> is used in the pass/fail file
///
template <typename T> bool PassFailDialog::AddSubMinMaxEntry(QTreeWidgetItem *parent, minMaxEntry<T>& ent)
{
    bool result = false;

    QObject::blockSignals(true);    // this prevents ItemChanged signal from being sent when we don't want it

    try
    {

//        if ( ent.b_isUsed)
        {
            //
            // Create a top-level widget item that shows min and max, where the value is "min, max" (e.g. "56, -49")
            //
            QString name(ent.group_name.c_str());
            QString min  = QString::number(ent.min_val);
            QString max  = QString::number(ent.max_val);
            QString value;
            value.append(min);
            value.append(", ");
            value.append(max);
            QStringList strings = (QStringList() << name << value);
            QTreeWidgetItem *minmax = new QTreeWidgetItem(strings);
            minmax->setHidden(false);
            parent->addChild(minmax);

            //
            // Now create 2 child widgets (one for min and one for max):
            //
            QString description = ent.min_description.c_str();
            name = ent.min_name.c_str();
            strings.clear();
            strings.append(name);
            strings.append(min);
            strings.append(description);
            QTreeWidgetItem *item = new QTreeWidgetItem(strings);
            item->setHidden(false);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            minmax->addChild(item);

            description = ent.max_description.c_str();
            name = ent.max_name.c_str();
            strings.clear();
            strings.append(name);
            strings.append(max);
            strings.append(description);
            item = new QTreeWidgetItem(strings);
            item->setHidden(false);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            minmax->addChild(item);
            result = true;
        }
    }
    catch (std::exception& e)
    {
        qDebug() << "Error in AddSubMinMaxEntry: "<< e.what();
        std::cerr << "Error in AddSubMinMaxEntry: "<< e.what() << std::endl;
    }

    QObject::blockSignals(false);

    return result;
}

///
/// Conditionally adds a vecEntry<T> as a subitem of a CMFCPropertyGridProperty if the vecEntry<T> is used in the pass/fail file
///
template <typename T> bool PassFailDialog::AddSubVecEntry(QTreeWidgetItem *parent, vecEntry<T>& ent)
{
    bool result = false;

    try
    {
  //      if ( ent.b_isUsed)
        {
            QString         name(ent.group_name.c_str());
            QStringList     vectorName(name);
            QString         vectorString;    // comma-separated list of data values (e.g. 2, 45, 67, 54)
            QTreeWidgetItem *vectorItem = new QTreeWidgetItem(vectorName);

            vectorItem->setHidden(false);
            parent->addChild(vectorItem);

            for (std::size_t i = 0 ; i < ent.value.size(); ++i)
            {
                QString desc;

                if ( i >= ent.sub_descriptions.size() || ent.sub_descriptions[i].empty())
                {
                    desc = ent.group_description.c_str();
                }
                else
                {
                    desc = ent.sub_descriptions[i].c_str();
                }

                if ( i >= ent.sub_names.size())
                {
                    name.clear();
                }
                else
                {
                    name = ent.sub_names[i].c_str();
                }
                QString value = QString::number(ent.value[i]);
                vectorString.append(value);

                if (i < ent.value.size()-1)
                {
                    vectorString.append(", ");
                }
                QStringList names = (QStringList() << name << value << desc);
                QTreeWidgetItem *item = new QTreeWidgetItem(names);
                item->setHidden(false);
                item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
                vectorItem->addChild(item);
            }

            vectorItem->setText(1, vectorString);
        }
    }
    catch (std::exception& e)
    {
        qDebug()  << "Error in AddSubVecEntry: " << e.what();
        std::cerr << "Error in AddSubVecEntry: " << e.what() << std::endl;
    }
    return result;
}

void PassFailDialog::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem*)
{
    if (current != nullptr)
    {
        ui->description->setText(current->text(2));
    }
    else
    {
        ui->description->clear();
    }

}

void PassFailDialog::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Qt::ItemFlags flags = item->flags();

    if (column == 1 && item->childCount() == 0)
    {
        item->setFlags(flags | Qt::ItemFlag::ItemIsEditable);
        ui->treeWidget->editItem(item, column);
    }
    else if (flags & Qt::ItemFlag::ItemIsEditable)
    {
        item->setFlags(flags ^ Qt::ItemFlag::ItemIsEditable);
    }
}


void PassFailDialog::on_treeWidget_itemChanged(QTreeWidgetItem *item, int /*column*/)
{
    //
    // The item text has changed.  If this is a vector or minmax item, we need to
    // update the text of the parent to reflect the change.  We can determine if
    // this is an aggregate item if its parent is not a top-level item.  A top-level
    // item is an item that has no parent.
    //
    QTreeWidgetItem *parent = item->parent();

    if (parent != nullptr && parent->parent() != nullptr)
    {
        //
        // We need to create a comma-delimited string of
        // the values of all of the parent's children.
        //
        QString string;
        int count = parent->childCount();

        for (int i = 0; i < count; ++i)
        {
            string += parent->child(i)->text(1);

            if (i < count-1)
            {
                string += ", ";
            }
        }

        parent->setText(1, string);
    }
}

///
/// When the OK button is clicked this function updates the entire m_settings class with the user supplied values
///
void PassFailDialog::on_buttonBox_accepted()
{
    for (int i = 0; i < ui->treeWidget->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem *parent = ui->treeWidget->topLevelItem(i);
        QString name = parent->text(0);

        if (name.compare(m_settings.sfrplus.name.c_str()) == 0)
        {
            UpdateSfrplus(parent);
        }
        else if (name.compare(m_settings.blemish.name.c_str()) == 0)
        {
            UpdateBlemish(parent);
        }
        else if (name.compare(m_settings.ois.name.c_str()) == 0)
        {
            UpdateOis(parent);
        }
        else if (name.compare(m_settings.other.name.c_str()) == 0)
        {
            UpdateOther(parent);
        }
    }
}


void PassFailDialog::UpdateBlemish(QTreeWidgetItem *blemish)
{
    int numChildren = blemish->childCount();

    for (int i = 0; i < numChildren; ++i)
    {
        QTreeWidgetItem *item = blemish->child(i);
        QString name = item->text(0);

        if (name.compare(m_settings.blemish.Blemish_maximum_count.group_name.c_str())==0) // Blemish_maximum_count
        {
            std::size_t numItems = static_cast<std::size_t>(item->childCount());
            m_settings.blemish.Blemish_maximum_count.value.resize(numItems);
            UpdateEntry(m_settings.blemish.Blemish_maximum_count, item);
        }
        else if (name.compare(m_settings.blemish.Blemish_size_pixels.group_name.c_str())==0) // Blemish_size_pixels
        {
            UpdateEntry(m_settings.blemish.Blemish_size_pixels, item);
        }
        else if (name.compare(m_settings.blemish.Dead_pixels_max.name.c_str())==0) // Dead_pixels_max
        {
            UpdateEntry(m_settings.blemish.Dead_pixels_max, item);
        }
        else if (name.compare(m_settings.blemish.Dead_pixel_clusters_max.name.c_str())==0) // Dead_pixel_clusters_max
        {
            UpdateEntry(m_settings.blemish.Dead_pixel_clusters_max, item);
        }
        else if (name.compare(m_settings.blemish.Defective_pixels_max_count.name.c_str())==0) // Defective_pixels_max_count
        {
           UpdateEntry(m_settings.blemish.Defective_pixels_max_count, item);
        }
        else if (name.compare(m_settings.blemish.Hot_pixel_clusters_max.name.c_str())==0) // Hot_pixel_clusters_max
        {
            UpdateEntry(m_settings.blemish.Hot_pixel_clusters_max, item);
        }
        else if (name.compare(m_settings.blemish.Hot_pixels_max.name.c_str())==0) // Hot_pixels_max
        {
            UpdateEntry(m_settings.blemish.Hot_pixels_max, item);
        }
        else if (name.compare(m_settings.blemish.Optical_center_offset_max.name.c_str())==0) // Optical_center_offset_max
        {
            UpdateEntry(m_settings.blemish.Optical_center_offset_max, item);
        }
        else if (name.compare(m_settings.blemish.Optical_center_offset_X_max.name.c_str())==0) // Optical_center_offset_X_max
        {
            UpdateEntry(m_settings.blemish.Optical_center_offset_X_max, item);
        }
        else if (name.compare(m_settings.blemish.Optical_center_offset_Y_max.name.c_str())==0) // Optical_center_offset_Y_max
        {
            UpdateEntry(m_settings.blemish.Optical_center_offset_Y_max, item);
        }
        else if (name.compare(m_settings.blemish.Relative_illumination_corner_diff_pct_max.name.c_str())==0) // Relative_illumination_corner_diff_pct_max
        {
            UpdateEntry(m_settings.blemish.Relative_illumination_corner_diff_pct_max, item);
        }
        else if (name.compare(m_settings.blemish.Relative_illumination_worst_corner_pct_min.name.c_str())==0) // Relative_illumination_worst_corner_pct_min
        {
            UpdateEntry(m_settings.blemish.Relative_illumination_worst_corner_pct_min, item);
        }
        else if (name.compare(m_settings.blemish.Uniformity_BoverG_corners_pct_max.name.c_str())==0) // Uniformity_BoverG_corners_pct_max
        {
            UpdateEntry(m_settings.blemish.Uniformity_BoverG_corners_pct_max, item);
        }
        else if (name.compare(m_settings.blemish.Uniformity_RoverG_corners_pct_max.name.c_str())==0) // Uniformity_RoverG_corners_pct_max
        {
            UpdateEntry(m_settings.blemish.Uniformity_RoverG_corners_pct_max, item);
        }
    }
}

void PassFailDialog::UpdateSfrplus(QTreeWidgetItem *sfrplus)
{
    int numChildren = sfrplus->childCount();

    for (int i = 0; i < numChildren; ++i)
    {
        QTreeWidgetItem *item = sfrplus->child(i);
        QString name = item->text(0);

        if (name.compare(m_settings.sfrplus.All_Edge_IDs_detected.name.c_str())==0) // All_Edge_IDs_detected
        {
            UpdateEntry(m_settings.sfrplus.All_Edge_IDs_detected, item);
        }
        else if (name.compare(m_settings.sfrplus.Bayer_decode.name.c_str())==0) // Bayer_decode
        {
            UpdateEntry(m_settings.sfrplus.Bayer_decode, item);
        }
        else if (name.compare(m_settings.sfrplus.Chart_mean_pixel_level_bounds.group_name.c_str())==0) // Chart_mean_pixel_level_bounds
        {
            UpdateEntry(m_settings.sfrplus.Chart_mean_pixel_level_bounds, item);
        }
        else if (name.compare(m_settings.sfrplus.Chart_radial_pixel_shift_max.name.c_str())==0) // Chart_radial_pixel_shift_max
        {
            UpdateEntry(m_settings.sfrplus.Chart_radial_pixel_shift_max, item);
        }
        else if (name.compare(m_settings.sfrplus.Color_expected_detected.name.c_str())==0) // Color_expected_detected
        {
            UpdateEntry(m_settings.sfrplus.Color_expected_detected, item);
        }
        else if (name.compare(m_settings.sfrplus.DeltaE_00_mean_max.name.c_str())==0) // DeltaE_00_mean_max
        {
            UpdateEntry(m_settings.sfrplus.DeltaE_00_mean_max, item);
        }
        else if (name.compare(m_settings.sfrplus.Convergence_angle_max.name.c_str())==0) // Convergence_angle_max
        {
            UpdateEntry(m_settings.sfrplus.Convergence_angle_max, item);
        }
        else if (name.compare(m_settings.sfrplus.FOV_degrees_diagonal_min.name.c_str())==0) // FOV_degrees_diagonal_min
        {
            UpdateEntry(m_settings.sfrplus.FOV_degrees_diagonal_min, item);
        }
        else if (name.compare(m_settings.sfrplus.High_pixel_saturation_fraction_max.name.c_str())==0) // High_pixel_saturation_fraction_max
        {
           UpdateEntry( m_settings.sfrplus.High_pixel_saturation_fraction_max, item);
        }
        else if (name.compare(m_settings.sfrplus.Horizontal_bars_OK_min.name.c_str())==0) // Horizontal_bars_OK_min
        {
            UpdateEntry(m_settings.sfrplus.Horizontal_bars_OK_min, item);
        }
        else if (name.compare(m_settings.sfrplus.Low_pixel_saturation_fraction_max.name.c_str())==0) // Low_pixel_saturation_fraction_max
        {
            UpdateEntry(m_settings.sfrplus.Low_pixel_saturation_fraction_max, item);
        }
        else if (name.compare(m_settings.sfrplus.Mirrored_chart.name.c_str())==0) // Mirrored_chart
        {
            UpdateEntry(m_settings.sfrplus.Mirrored_chart, item);
        }
        else if (name.compare(m_settings.sfrplus.MTF50P_CP_weighted_mean_min.name.c_str())==0) // MTF50P_CP_weighted_mean_min
        {
            UpdateEntry(m_settings.sfrplus.MTF50P_CP_weighted_mean_min, item);
        }
        else if (name.compare(m_settings.sfrplus.MTF50P_ratio_min.name.c_str())==0) // MTF50P_ratio_min
        {
            UpdateEntry(m_settings.sfrplus.MTF50P_ratio_min, item);
        }
        else if (name.compare(m_settings.sfrplus.passfail_ini_file_date.name.c_str())==0) // passfail_ini_file_date
        {
            UpdateEntry(m_settings.sfrplus.passfail_ini_file_date, item);
        }
        else if (name.compare(m_settings.sfrplus.Rotation_degrees_max.name.c_str())==0) // Rotation_degrees_max
        {
            UpdateEntry(m_settings.sfrplus.Rotation_degrees_max, item);
        }
        else if (name.compare(m_settings.sfrplus.Secondary_readout_1_center_mean_min.name.c_str())==0) // Secondary_readout_1_center_mean_min
        {
            UpdateEntry(m_settings.sfrplus.Secondary_readout_1_center_mean_min, item);
        }
        else if (name.compare(m_settings.sfrplus.Secondary_readout_1_outer_mean_min.name.c_str())==0) // Secondary_readout_1_outer_mean_min
        {
            UpdateEntry(m_settings.sfrplus.Secondary_readout_1_outer_mean_min, item);
        }
        else if (name.compare(m_settings.sfrplus.Secondary_readout_1_outer_min_min.name.c_str())==0) // Secondary_readout_1_outer_min_min
        {
            UpdateEntry(m_settings.sfrplus.Secondary_readout_1_outer_min_min, item);
        }
        else if (name.compare(m_settings.sfrplus.Secondary_readout_1_outer_quadrant_delta_max.name.c_str())==0) // Secondary_readout_1_outer_quadrant_delta_max
        {
            UpdateEntry(m_settings.sfrplus.Secondary_readout_1_outer_quadrant_delta_max, item);
        }
        else if (name.compare(m_settings.sfrplus.Secondary_readout_1_outer_quadrant_mean_min_min.name.c_str())==0) // Secondary_readout_1_outer_quadrant_mean_min_min
        {
            UpdateEntry(m_settings.sfrplus.Secondary_readout_1_outer_quadrant_mean_min_min, item);
        }
        else if (name.compare(m_settings.sfrplus.Secondary_readout_2_center_mean_min.name.c_str())==0) // Secondary_readout_2_center_mean_min
        {
            UpdateEntry(m_settings.sfrplus.Secondary_readout_2_center_mean_min, item);
        }
        else if (name.compare(m_settings.sfrplus.Secondary_readout_2_outer_mean_min.name.c_str())==0) // Secondary_readout_2_outer_mean_min
        {
            UpdateEntry(m_settings.sfrplus.Secondary_readout_2_outer_mean_min, item);
        }
        else if (name.compare(m_settings.sfrplus.Secondary_readout_2_outer_min_min.name.c_str())==0) // Secondary_readout_2_outer_min_min
        {
            UpdateEntry(m_settings.sfrplus.Secondary_readout_2_outer_min_min, item);
        }
        else if (name.compare(m_settings.sfrplus.Secondary_readout_2_outer_quadrant_delta_max.name.c_str())==0) // Secondary_readout_2_outer_quadrant_delta_max
        {
            UpdateEntry(m_settings.sfrplus.Secondary_readout_2_outer_quadrant_delta_max, item);
        }
        else if (name.compare(m_settings.sfrplus.Secondary_readout_2_outer_quadrant_mean_min_min.name.c_str())==0) // Secondary_readout_2_outer_quadrant_mean_min_min
        {
            UpdateEntry(m_settings.sfrplus.Secondary_readout_2_outer_quadrant_mean_min_min, item);
        }
        else if (name.compare(m_settings.sfrplus.Stepchart_expected_detected.name.c_str())==0) // Stepchart_expected_detected
        {
            UpdateEntry(m_settings.sfrplus.Stepchart_expected_detected, item);
        }
        else if (name.compare(m_settings.sfrplus.upside_down.name.c_str())==0) // upside_down
        {
           UpdateEntry( m_settings.sfrplus.upside_down, item);
        }
    }
}

void PassFailDialog::UpdateOis(QTreeWidgetItem *ois)
{
    int numChildren = ois->childCount();

    for (int i = 0; i < numChildren; ++i)
    {
        QTreeWidgetItem *item = ois->child(i);
        QString name = item->text(0);

        if (name.compare(m_settings.ois.L_MTF50_delta2_gain_summary_all_dB_min.name.c_str())==0) // L_MTF50_delta2_gain_summary_all_dB_min
        {
            UpdateEntry(m_settings.ois.L_MTF50_delta2_gain_summary_all_dB_min, item);
        }
        else if (name.compare(m_settings.ois.R_improve_ALL_dB_min.name.c_str())==0) // R_improve_ALL_dB_min
        {
            UpdateEntry(m_settings.ois.R_improve_ALL_dB_min, item);
        }
        else if (name.compare(m_settings.ois.R_improve_H_dB_min.name.c_str())==0) // R_improve_H_dB_min
        {
            UpdateEntry(m_settings.ois.R_improve_H_dB_min, item);
        }
        else if (name.compare(m_settings.ois.R_improve_V_dB_min.name.c_str())==0) // R_improve_V_dB_min
        {
            UpdateEntry(m_settings.ois.R_improve_V_dB_min, item);
        }
    }
}

void PassFailDialog::UpdateOther(QTreeWidgetItem *other)
{
    std::size_t numChildren = static_cast<std::size_t>(other->childCount());

    for (std::size_t i = 0; i < numChildren; ++i)
    {
        QTreeWidgetItem *item = other->child(static_cast<int>(i)); // Get the sub item
        UpdateEntry(m_settings.other.ent_vec[i], item);
    }
}

template <typename T> void PassFailDialog::UpdateEntry(entry<T>& ent, QTreeWidgetItem *item)
{
    if (item != nullptr)
    {
        str_to_value(item->text(1), ent.value);
    }
}

template <typename T> void PassFailDialog::UpdateEntry(minMaxEntry<T>& ent, QTreeWidgetItem *parent)
{
    str_to_value(parent->child(0)->text(1), ent.min_val);
    str_to_value(parent->child(1)->text(1), ent.max_val);
}

template <typename T> void PassFailDialog::UpdateEntry(vecEntry<T>& ent, QTreeWidgetItem *parent)
{
    std::size_t cnt = ent.value.size();

    for (std::size_t i = 0; i < cnt; ++i)
    {
        QTreeWidgetItem *item = parent->child(static_cast<int>(i));

        if (item != nullptr)
        {
            str_to_value(item->text(1), ent.value[i]);
        }
    }
}
